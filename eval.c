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

#include "types.h"
#include "read.h"
#include "eval.h"
#include "util.h"
#include "debug.h"

bool streq( char *a, char *b ) {
    return strcmp( a, b ) == 0;
}

/*
 * Returns 'true' if the list 'l' is the function 'function'.
 * \param l checks if this function is 'function'
 * \param function checks if 'l' is this function
 * \return 'true' if the list 'l' is the function 'function'
 */
bool is_function( list *l, symbol function ) {
    return strcmp( l->car->data.s, function ) == 0;
}

int num_args( list *l ) {
    return length( l ) - 1;
}

object *quote( object *o ) {
    object *result = new_symbol_object();
    // printf( "*** 'quote' has [%d] arguments\n", num_args( o->data.l ) );
    if ( num_args( o->data.l ) == 1 ) {
        result = o->data.l->cdr->car;
    } else {
        error( "'quote' expects exactly one argument" );
    }
    return result;
}

/*
 * Evaluates the list 'l'.
 * \param l the list to evaluate
 * \return the value of 'l'
 */
object *eval_object( object *o ) {
    object *eval = o;

    if ( LIST == o->type ) {
        printd( "Read list" );
        if ( is_null( o->data.l ) ) {
            printd( "Read empty list" );
            eval = o;
        } else if ( is_function( o->data.l, "quote" ) ) {
            eval = quote( o );
        } else {
            error( "Unrecognized function" );
        }
    } else {
        printd( "Read symbol" );
        if ( streq( o->data.s, "quit" ) ) {
            printf( "\nGoodbye." );
            exit( 0 );
        }
        eval = o;
    }

    return eval;
}

object *eval( object *o ) {
    printd( "Evaluating symbol" );
    return eval_object( o );
    printd( "Finished evaluating symbol" );
}
