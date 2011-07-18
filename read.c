#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "read.h"

void error( char *message ) {
    fprintf( stderr, "[!] Error: %s\n", message );
    exit( 1 );
}

list *EMPTY_LIST;

bool is_null( list *l ) {
    return EMPTY_LIST == l;
}

symbol new_symbol() {
    symbol s = malloc( sizeof( char ) * SYMBOL_LEN );
    if ( NULL == s ) error( "Out of memory: allocating symbol" );
    return s;
}

list *new_list() {
    list *l = malloc( sizeof( list ) );
    if ( NULL == l ) error( "Out of memory: allocating list" );
    l->car = new_symbol();
    return l;
}

void init() {
    EMPTY_LIST = new_list();
}

bool in_chars( char c, char *cs ) {
    return '\0' == c ? false : c == *cs || in_chars( c, cs++ );
}

bool is_white( int c ) {
    return ' ' == c || '\t' == c || '\n' == c || EOF == c;
    // return in_chars( c, " \t\n\0" );
}

bool is_delim( int c ) {
    return '(' == c || ')' == c;
    // return in_chars( c, "()\0" );
}

bool is_symbolic( int c ) {
    return ! is_white( c ) && ! is_delim( c );
}

void eat_whitespace( FILE *in ) {
    int c;
    do {
        c = getc( in );
    } while( c != EOF && is_white( c ) );
    ungetc( c, in );
}

void eat( FILE *in, char expected ) {
    int c = getc( in );
    if ( expected != c ) {
        fprintf( stderr, "[!] Expected '%c', got '%c'\n", expected, c );
        exit( 2 );
    }
}

symbol read_symbol( FILE *in ) {
    int c, i = 0;
    symbol s = new_symbol();

    while ( is_symbolic( c = getc( in ) ) ) s[ i++ ] = c;
    s[ i ] = '\0';
    ungetc( c, in );

    return s;
}

list *read_list( FILE *in ) {
    int c;
    list *l = new_list();

    eat_whitespace( in );
    eat( in, '(' );
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
            eat( in, ')' );
        }
    }

    return l;
}

list *read( FILE *in ) {
    return read_list( in );
}
