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

object *eval_object( object *o );

object *quote( object *o ) {
    object *result;
    // printf( "*** 'quote' has [%d] arguments\n", num_args( o->data.l ) );
    if ( num_args( o->data.l ) == 1 ) {
        result = o->data.l->cdr->car;
    } else {
        error( "'quote' expects exactly one argument" );
    }
    return result;
}

bool is_atomic( object *o ) {
    // Short circuiting stops this throwing a type error
    return SYMBOL == o->type || is_null( o->data.l );
}

object *true_object() {
    object *t = new_symbol_object();
    t->data.s = TRUE;
    return t;
}

bool atom( object *o ) {
    bool atom;
    if ( num_args( o->data.l ) == 1 ) {
         atom = is_atomic( eval_object( o->data.l->cdr->car ) );
    } else {
        error( "'atom' expects exactly one argument" );
    }
    return atom;
}

bool eq_object( object *, object * );

bool eq_list( list *a, list *b ) {
    bool eq = eq_object( a->car, b->car );

    if ( is_null( a ) && is_null( b ) ) {
        eq &= true;
    } else if ( is_null( a ) || is_null( b ) ) {
        eq = false;
    } else {
        eq &= eq_list( a->cdr, b->cdr );
    }

    return eq;
}

bool eq_object( object *a, object *b ) {
    bool eq = false;
    if ( a->type == b->type ) {
        if ( SYMBOL == a->type ) {
            eq = strcmp( a->data.s, b->data.s ) == 0;
        } else {
            eq = eq_list( a->data.l, b->data.l );
        }
    }
    return eq;
}

bool eq( object *o ) {
    bool result = false;
    if ( num_args( o->data.l ) == 2 ) {
        object *a = eval_object( second( o ) );
        object *b = eval_object( third( o ) );
        result = eq_object( a, b );
    } else {
        error( "'eq' expects exactly two arguments" );
    }
    return result;
}

object *bool_to_object( bool b ) {
    return b ? true_object() : new_list_object();
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
        list *l = o->data.l;

        if ( is_null( l ) ) {
            printd( "Read empty list" );
            eval = o;
        } else if ( is_function( l, "quote" ) ) {
            eval = quote( o );
        } else if ( is_function( l, "atom" ) ) {
            eval = bool_to_object( atom( o ) );
        } else if ( is_function( l, "eq" ) ) {
            eval = bool_to_object( eq( o ) );
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
