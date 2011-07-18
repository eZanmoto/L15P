#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "types.h"
#include "read.h"
#include "util.h"

/*
 * Represents the empty list.
 */
list *EMPTY_LIST;

/*
 * Returns 'true' if 'l' is the empty list.
 * \param l the list to check
 * \return 'true' if 'l' is the empty list
 */
bool is_null( list *l ) {
    return EMPTY_LIST == l;
}

/*
 * Initializes constants.
 */
void init() {
    EMPTY_LIST = new_list();
}

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
    do {
        c = getc( in );
    } while( c != EOF && is_white( c ) );
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

/*
 * Reads the next symbol from 'in'.
 * \param in the buffer to read the next symbol from
 */
symbol read_symbol( FILE *in ) {
    int c, i = 0;
    symbol s = new_symbol();

    while ( is_symbolic( c = getc( in ) ) ) s[ i++ ] = c;
    s[ i ] = '\0';
    ungetc( c, in );

    eat_whitespace( in );

    return s;
}

/*
 * Reads the next list from 'in'.
 * \param in the buffer to read the next list from
 */
list *read_list( FILE *in ) {
    int c;
    list *l = new_list();

    eat_whitespace( in );
    match( in, '(' );
    c = getc( in );

    if ( ')' == c ) {
        l = EMPTY_LIST;
    } else {
        ungetc( c, in );

        l->car = read_symbol( in );
        if ( ')' ) {
            l->cdr = EMPTY_LIST;
        } else {
            l->cdr = read_list( in );
            match( in, ')' );
        }
    }

    return l;
}

/*
 * Reads a list from the buffer 'in'.
 * \param the buffer to read the next list from
 */
list *read( FILE *in ) {
    return read_list( in );
}
