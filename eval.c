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
#include "print.h"
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
        result = o;
    }
    return result;
}

object *true_object() {
    object *t = new_symbol_object();
    t->data.s = TRUE;
    return t;
}

bool is_atomic( object *o ) {
    // Short circuiting stops this throwing a type error
    return SYMBOL == o->type || is_null( o->data.l );
}

bool atom( object *o ) {
    bool result = false;
    if ( num_args( o->data.l ) == 1 ) {
         result = is_atomic( eval_object( o->data.l->cdr->car ) );
    } else {
        error( "'atom' expects exactly one argument" );
    }
    return result;
}

bool eq_list( list *a, list *b );
bool eq_object( object *, object * );

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

object *cons( object *o ) {
    object *result;
    if ( num_args( o->data.l ) == 2 ) {
        output( 2, ">Evaluating cons" );
        result = new_object();
        result->type = LIST;
        result->data.l = new_list();
        output( 3, ">>Evaluating head" );
        object *secondParameter = eval_object( second( o ) );
        output( 3, "<<Evaluating head" );
        output( 3, ">>>Populating head" );
        result->data.l->car = secondParameter;
        output( 3, "<<<Populating head" );
        output( 3, ">>Evaluating tail" );
        object *thirdParameter = eval_object( third( o ) );
        if ( thirdParameter->type == LIST ) {
            output( 3, ">>Populating tail" );
            result->data.l->cdr = thirdParameter->data.l;
        } else {
            error( "The second argument to 'cons' should be a list" );
            output( 3, ">>Tail is empty" );
            result->data.l->cdr = EMPTY_LIST;
        }
    } else {
        error( "'cons' expects exactly two arguments" );
        result = o;
    }
    return result;
}

object *car( object *o ) {
    object *result;

    if ( num_args( o->data.l ) == 1 ) {
        result = eval_object( second( o ) );
        if ( result->type == LIST ) {
            if ( !is_null( result->data.l ) ) {
                result = result->data.l->car;
            } else {
                error( "The empty list doesn't have a 'car'" );
            }
        } else {
            error( "The argument to 'car' should be a list" );
            result = new_list_object();
        }
    } else {
        error( "'car' expects exactly one argument" );
        result = o;
    }

    return result;
}

object *cdr( object *o ) {
    object *result;

    if ( num_args( o->data.l ) == 1 ) {
        result = eval_object( second( o ) );
        if ( result->type == LIST ) {
            if ( !is_null( result->data.l ) ) {
                list *l = result->data.l->cdr;

                result = new_object();
                result->type = LIST;
                result->data.l = l;
            } else {
                error( "The empty list doesn't have a 'cdr'" );
            }
        } else {
            error( "The argument to 'cdr' should be a list" );
            result = new_list_object();
        }
    } else {
        error( "'cdr' expects exactly one argument" );
        result = o;
    }

    return result;
}

bool eval_cond_test( object *o ) {
    bool result = true;

    if ( LIST == o->type ) {
        output( 3, ">>>Evaluating test" );
        object *eval = eval_object( o );
        output( 3, "<<<Evaluating test" );

        result = ! ( LIST == eval->type && is_null( eval->data.l ) );

        // free( eval );
    }

    return result;
}

object *eval_cond_pair( list *l ) {
    object *result;

    if ( length( l ) == 2 ) {
        if ( eval_cond_test( l->car ) ) {
            output( 2, ">>Test was true, evaluating rest" );
            result = eval_object( l->cdr->car );
            // printf( "*** Result: " );
            // print( result );
            // printf( "\n*** Car: " );
            // print( l->car );
            // printf( "\n" );
            output( 2, ">>Test was true, evaluating rest" );
        } else {
            output( 2, ">>Test was false" );
            result = new_list_object();
            output( 2, "<<Test was false" );
        }
    } else {
        error( "List arguments to 'cond' must be have two elements" );
        result = new_list_object();
    }

    printf( "\n*** 'eval_list' returning: " );
    print( result );
    printf( "\n" );

    return result;
}

object *eval_cond( list *l ) {
    object *result;

    if ( is_null( l ) ) {
        result = new_list_object();
    } else {
        output( 2, ">>Evaluating list argument" );
        object *eval = eval_object( l->car );
        output( 2, "<<Evaluating list argument" );

        if ( LIST == eval->type ) {
            output( 3, ">>>Evaluating pair" )
            result = eval_cond_pair( eval->data.l );
            output( 3, "<<<Evaluating pair" )
            if ( LIST == result->type && is_null( result->data.l ) ) {
                output( 2, ">>Evaluating next argument" );
                result = eval_cond( l->cdr );
                output( 2, "<<Evaluating next argument" );
            }
        } else {
            error( "Arguments to 'cond' must be lists" );
            result = new_list_object();
        }

        // free( eval );
    }

    printf( "\n*** 'eval_cond' returning: " );
    print( result );
    printf( "\n" );

    return result;
}

object *cond ( object *o ) {
    object *result;

    if ( num_args( o->data.l ) >= 1 ) {
        result = eval_cond( o->data.l->cdr );
    } else {
        error( "'cond' expects at least one argument" );
        result = o;
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
        } else if ( is_function( l, "cons" ) ) {
            eval = cons( o );
        } else if ( is_function( l, "car" ) ) {
            eval = car( o );
        } else if ( is_function( l, "cdr" ) ) {
            eval = cdr( o );
        } else if ( is_function( l, "cond" ) ) {
            eval = cond( o );
        } else {
            error( "Unrecognized function" );
        }
    } else {
        printd( "Read symbol" );
        if ( streq( o->data.s, "quit" ) ) {
            printf( "\nGoodbye." );
            exit( 0 );
        } else {
            error( "Naked symbol" );
            // printf( "[!] Symbol [%s]\n", o->data.s );
            eval = o;
        }
    }

    return eval;
}

object *eval( object *o ) {
    printd( "Evaluating object" );
    o = eval_object( o );
    printd( "Finished evaluating object" );
    return o;
}
