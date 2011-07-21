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

#include "types.h"
#include "read.h"
#include "print.h"
#include "debug.h"

void print_object( object * );

void print_list( list *l ) {
    if ( ! is_null( l ) ) {
        output( 1, "Printing regular list" );
        print_object( l->car );
        print_list( l->cdr );
    } else {
        output( 1, "Printing empty list" );
    }
}

void print_object( object *o ) {
    if ( o->type == SYMBOL ) {
        output( 2, "Printing symbol" );
        printf( " %s ", o->data.s );
    } else {
        output( 2, "Printing list" );
        printf( "(" );
        print_list( o->data.l );
        printf( ")" );
    }
}

void print( object *o ) {
    output( 3, "Printing object" );
    print_object( o );
    free( o );
    output( 3, "Finished printing object" );
}
