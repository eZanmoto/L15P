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

object *new_symbol_object() {
    object *o = new_object();
    o->type = SYMBOL;
    o->data.s = new_symbol();
    return o;
}

object *new_list_object() {
    object *o = new_object();
    o->type = LIST;
    o->data.l = EMPTY_LIST;
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
    l->car = new_symbol_object();
    l->cdr = EMPTY_LIST;
    return l;
}

bool is_null( list *l ) {
    return EMPTY_LIST == l;
}
