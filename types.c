#include <stdlib.h>

#include "types.h"
#include "util.h"

/*
 * Returns a new symbol.
 * \return a new symbol
 */
symbol new_symbol() {
    symbol s = malloc( sizeof( char ) * MAX_SYMBOL_LEN );
    if ( NULL == s ) fatal( "Out of memory: allocating symbol" );
    s[ 0 ] = '\0';
    return s;
}

/*
 * Represents the empty list.
 */
list *EMPTY_LIST;

void init_types() {
    EMPTY_LIST = new_list();
}

object *new_object() {
    object *o = malloc( sizeof( object ) );
    if ( NULL == o ) fatal( "Out of memory: allocating object" );
    return o;
}

bool is_symbol( object *o ) {
    return SYMBOL == o->type;
}

bool is_list( object *o ) {
    return LIST == o->type;
}

/*
 * Returns a new list.
 * \return a new list
 */
list *new_list() {
    list *l = malloc( sizeof( list ) );
    if ( NULL == l ) fatal( "Out of memory: allocating list" );
    l->car = new_object();
    return l;
}

bool is_null( list *l ) {
    return EMPTY_LIST == l;
}
