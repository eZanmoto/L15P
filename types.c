#include <stdlib.h>

#include "types.h"
#include "util.h"

/*
 * Returns a new symbol.
 * \return a new symbol
 */
symbol new_symbol() {
    symbol s = malloc( sizeof( char ) * MAX_SYMBOL_LEN );
    if ( NULL == s ) error( "Out of memory: allocating symbol" );
    return s;
}

/*
 * Returns a new list.
 * \return a new list
 */
list *new_list() {
    list *l = malloc( sizeof( list ) );
    if ( NULL == l ) error( "Out of memory: allocating list" );
    l->car = new_symbol();
    return l;
}
