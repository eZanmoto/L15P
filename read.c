/*
 * L15P - a lisp interpreter aiming to comply with the 1.5 specification.
 * Copyright (C) 2011 Sean Kelleher (http://ezanmoto.wordpress.com/)
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public
 * License version 3 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License version 3 for more details.
 *
 * You should have received a copy of the GNU Affero General Public
 * License version 3 along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "types.h"
#include "read.h"
#include "util.h"
#include "debug.h"

/*
 * Returns 'true' if 'c' is in 'cs'.
 * \param c the character to look for
 * \param cs checks these characters for 'c'
 * \return 'true' if 'c' is in 'cs'
 */
bool in_chars( char c, char *cs ) {
    return '\0' == c ? false : c == *cs || in_chars( c, cs++ );
}

/*
 * Returns 'true' if 'c' is a whitespace character.
 * \param c checks if this is a whitespace character
 * \return 'true' if 'c' is a whitespace character
 */
bool is_white( int c ) {
    return ' ' == c || '\t' == c || '\n' == c;
    // return in_chars( c, " \t\n\0" );
}

/*
 * Returns 'true' if 'c' is a delimiter character.
 * \param c checks if this is a delimiter character
 * \return 'true' if 'c' is a delimiter character
 */
bool is_delim( int c ) {
    return '(' == c || ')' == c;
    // return in_chars( c, "()\0" );
}

/*
 * Returns 'true' if 'c' can be part of a symbol.
 * \param c checks if this can be part of a symbol
 * \return 'true' if 'c' can be part of a symbol
 */
bool is_symbolic( int c ) {
    return ! is_white( c ) && ! is_delim( c );
}

/*
 * Removes whitespace from the start of 'in'.
 * \param in removes whitespace from the start of this file
 */
void eat_whitespace( FILE *in ) {
    int c;
    output( 2, ">Eating whitespace" );
    do {
        c = getc( in );
    } while( c != EOF && is_white( c ) );
    output( 2, "<Eating whitespace" );
    ungetc( c, in );
}

/*
 * Signals an error if the next character of 'in' is not 'expected'.
 * \param in the buffer to check for 'expected'
 * \param expected the character to check for
 */
void match( FILE *in, char expected ) {
    int c = getc( in );
    if ( expected != c ) {
        fprintf( stderr, "[!] Expected '%c', got '%c'\n", expected, c );
        exit( 2 );
    }
}

list *read_list( FILE *in );
symbol read_symbol( FILE *in );

/*
 * Reads the next object from 'in'.
 */
object *read_object( FILE *in ) {
    int c;
    object *o = new_object();

    eat_whitespace( in );
    c = getc( in );

    if ( '(' == c ) {
        output( 2, ">>Reading list" );
        o->type = LIST;
        o->data.l = read_list( in );
        output( 2, "<<Reading list" );
    } else {
        output( 2, ">>Reading symbol" );
        ungetc( c, in );
        o->type = SYMBOL;
        o->data.s = read_symbol( in );
        output( 2, "<<Reading symbol" );
    }

    return o;
}

/*
 * Reads the next symbol from 'in'.
 * \param in the buffer to read the next symbol from
 */
symbol read_symbol( FILE *in ) {
    int c, i = 0;
    symbol s = new_symbol();

    output( 1, ">>>Reading symbol" );

    while ( is_symbolic( c = getc( in ) ) ) s[ i++ ] = c;
    s[ i ] = '\0';
    ungetc( c, in );
    output( 1, "<<<Reading symbol" );

    return s;
}

/*
 * Converts the array 'os' to a list
 * \param os the array to convert to a list
 * \param size the size of the list
 * \return a list holding the elements of 'os'
 */
list *objects_to_list( object **os, int size ) {
    list *l = new_list();
    l->car = *os;
    l->cdr = 0 == size ? EMPTY_LIST : objects_to_list( ++os, --size );
    return l;
}

#define MAX_OBJECTS 32

/*
 * Reads the next list from 'in'.
 * \param in the buffer to read the next list from
 */
list *read_list( FILE *in ) {
    int c, i = 0;
    list *l = new_list();
    object *os[ MAX_OBJECTS ];

    eat_whitespace( in );

    if ( ( c = getc( in ) ) == ')' ) {
        output( 1, ">>>Read empty list" );
        l = EMPTY_LIST;
        output( 1, "<<<Read empty list" );
    } else {
        ungetc( c, in );
        output( 1, ">>>Read regular list" );
        output( 4, ">>>>[1]Read object" );
        l->car = read_object( in );
        output( 4, "<<<<[1]Read object" );
        while ( ( c = getc( in ) ) != ')' ) {
            ungetc( c, in );
            eat_whitespace( in );
            output( 4, ">>>>[2]Read object" );
            os[ i ] = read_object( in );
            output( 4, "<<<<[2]Read object" );
            i++;
        }
        l->cdr = objects_to_list( os, i );
        output( 1, "<<<Read regular list" );
    }
    output( 1, "<<<Read regular list" );

    return l;
}

/*
 * Reads a list from the buffer 'in'.
 * \param the buffer to read the next list from
 */
object *read( FILE *in ) {
    output( 3, ">Reading object" );
    return read_object( in );
    output( 3, "<Reading object" );
}
