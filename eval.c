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

/*
 * Returns 'true' if the list 'l' is the function 'function'.
 * \param l checks if this function is 'function'
 * \param function checks if 'l' is this function
 * \return 'true' if the list 'l' is the function 'function'
 */
bool is_function( list *l, symbol function ) {
    return streq( l->car->data.s, function );
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

bool eval_cond_pair( list *l ) {
    bool result = false;

    if ( length( l ) == 2 ) {
        result = eval_cond_test( l->car );
    } else {
        error( "List arguments to 'cond' must be have two elements" );
    }

    return result;
}

object *eval_cond( list *l ) {
    object *result;

    if ( is_null( l ) ) {
        result = new_list_object();
    } else {
        object *eval = l->car;

        if ( LIST == eval->type ) {
            output( 3, "*** Evaluating pair" )
            if ( eval_cond_pair( eval->data.l ) ) {
                result = eval_object( eval->data.l->cdr->car );
            } else {
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

    // printf( "\n*** 'eval_cond' returning: " );
    // print( result );
    // printf( "\n" );

    return result;
}

object *cond( object *o ) {
    object *result;

    if ( num_args( o->data.l ) >= 1 ) {
        result = eval_cond( o->data.l->cdr );
    } else {
        error( "'cond' expects at least one argument" );
        result = o;
    }

    return result;
}

int pindex( symbol pname, list *plist, int cur_index ) {
    int index = -1;

    if ( ! is_null( plist ) ) {
        if ( SYMBOL == plist->car->type ) {
            output( 3, ">>>Checking head" );
            bool found = streq( plist->car->data.s, pname );
            output( 3, "<<<Checking head" );
            if ( found ) {
                index = cur_index;
            } else {
                output( 3, ">>>Checking tail" );
                index = pindex( pname, plist->cdr, cur_index + 1 );
                output( 3, "<<<Checking tail" );
            }
        } else {
            error( "Parameter list may only contain symbols" );
        }
    } else {
        output( 1, "*** Symbol isn't parameter" );
        // printf( "*** Parameter [%s]\n", pname );
    }

    return index;
}

int parameter_index( symbol pname, list *plist ) {
    return pindex( pname, plist, 0 );
}

list *expr_list( list *param_list, list *expr ) {

    if ( ! is_null( expr ) ) {
        if ( SYMBOL == expr->car->type ) {
            output( 3, ">>>Checking for parameter" );
            int i = parameter_index( expr->car->data.s, param_list );
            if ( i >= 0 ) {
                expr->car = new_parameter_object();
                expr->car->data.p = i;
            }
            output( 3, "<<<Checking for parameter" );
        } else if ( LIST == expr->car->type ) {
            expr->car->data.l = expr_list( param_list, expr->car->data.l );
        }
        expr->cdr = expr_list( param_list, expr->cdr );
    } else {
        output( 3, "*** Finished evaluating lambda list" );
    }

    return expr;
}

/*
 * Traverse the expression tree 'expr' and replace all symbols that occur in
 * 'param_list' with a parameter number.
 */
object *new_funct( list *param_list, object *expr ) {
    object *funct = new_function_object();

    if ( LIST == expr->type ) {
        output( 2, ">>Evaluating lambda list" );
        funct->data.l = expr_list( param_list, expr->data.l );
        output( 2, "<<Evaluating lambda list" );
    } else {
        error( "The expression to 'lambda' must be a list" );
        funct->data.l = EMPTY_LIST;
    }

    return funct;
}

object *lambda( object *o ) {
    object *funct;

    if ( num_args( o->data.l ) == 2 ) {
        object *temp = second( o );

        if ( LIST == temp->type ) {
            list *param_list = temp->data.l;
            object *expr = third( o );
            output( 1, ">Evaluating lambda" );
            funct = new_funct( param_list, expr );
            output( 1, "<Evaluating lambda" );
        } else {
            error( "The first parameter to 'lambda' must be a list" );
            funct = new_list_object();
        }
    } else {
        error( "'lambda' expects exactly two parameters" );
        funct = new_list_object();
    }

    return funct;
}

object *bool_to_object( bool b ) {
    return b ? true_object() : new_list_object();
}

object *param_with_index( int index, list *param_list ) {
    object *param;

    if ( is_null( param_list ) ) {
        error( "Parameter index out of bounds" );
        param = new_list_object();
    } else {
        if ( 0 == index ) {
            param = param_list->car;
        } else {
            param = param_with_index( index - 1, param_list->cdr );
        }
    }

    return param;
}

list *funct_list( list *param_list, list *funct ) {

    if ( ! is_null( funct ) ) {
        if ( PARAMETER == funct->car->type ) {
            funct->car = param_with_index( funct->car->data.p, param_list );
        } else if ( LIST == funct->car->type ) {
            funct->car->data.l = funct_list( param_list, funct->car->data.l );
        }
        funct->cdr = funct_list( param_list, funct->cdr );
    }

    return funct;
}

/*
 * Traverse the expression tree 'funct' and replace all parameter numbers with
 * their correspondant from 'param_list'.
 */
object *eval_funct( list *param_list, object *funct ) {
    object *expr = new_object();
    expr->type   = LIST;
    // TODO: evaluate param_list
    expr->data.l = funct_list( param_list, funct->data.l );
    return expr;
}

#define MAX_FUNCTIONS 256

symbol *FunctionName;
object **Function;
int NumFunctions;

void init_eval() {
    FunctionName = malloc( sizeof( symbol ) * MAX_FUNCTIONS );
    Function = malloc( sizeof( object* ) * MAX_FUNCTIONS );
    NumFunctions = 0;
}

object *label( object *o ) {
    object *result;

    if ( num_args( o->data.l ) == 2 ) {
        result = second( o );
        if ( SYMBOL == result->type ) {

            symbol name = result->data.s;
            output( 3, "*** Got function name" );
            object *funct = eval_object( third( o ) );
            output( 3, "*** Got function" );
            if ( FUNCTION == funct->type ) {
                output( 3, ">>>Assigning new name" );
                FunctionName[ NumFunctions ] = name;
                output( 3, "<<<Assigning new name" );
                output( 3, ">>>Assigning new function" );
                Function[ NumFunctions++ ] = funct;
                output( 3, "<<<Assigning new function" );
            } else {
                error( "The second argument to 'label' must be a function" );
                result = new_list_object();
            }
        } else {
            error( "The first argument to 'label' must be a symbol" );
            result = new_list_object();
        }
    } else {
        error( "'label' expects exactly two arguments" );
        result = o;
    }

    return result;
}

int function_index( list *l ) {
    int i;
    bool found;
    output( 2, ">>Looking for function" );
    for ( i = 0; i < NumFunctions && ! found; i++ ) {
        found = streq( l->car->data.s, FunctionName[ i ] );
    }
    output( 2, "<<Looking for function" );
    return ( found ? i : 0 ) - 1;
}

object *_eval_object( object *o, bool is_head ) {
    object *eval = o;

    if ( LIST == o->type ) {
        printd( "Read list" );
        o->data.l->car = _eval_object( o->data.l->car, true );
        list *l = o->data.l;
        int findex;

        if ( is_null( l ) ) {
            printd( "Read empty list" );
            warning( "Naked symbol" );
            eval = o;
        } else if ( FUNCTION == o->data.l->car->type ) {
            object *expr = eval_funct( o->data.l->cdr, o->data.l->car );
            eval = eval_object( expr );
            free( expr );
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
        } else if ( is_function( l, "lambda" ) ) {
            eval = lambda( o );
        } else if ( is_function( l, "label" ) ) {
            eval = label( o );
        } else if ( ( findex = function_index( l ) ) >= 0 ) {
            output( 1, ">Executing labelled function" );
            // printf( "findex: %d\n", findex );
            // print( Function[ findex ] );
            output( 2, ">>Copying object" );
            object *funct = copy_object( Function[ findex ] );
            output( 2, "<<Copying object" );
            object *expr = eval_funct( o->data.l->cdr, funct );
            eval = eval_object( expr );
            output( 1, "<Executing labelled function" );
        } else {
            error( "Unrecognized function" );
        }
    } else if ( FUNCTION == o->type ) {
        eval = o;
    } else {
        printd( "Read symbol" );
        if ( streq( o->data.s, "quit" ) ) {
            printf( "\nGoodbye.\n" );
            exit( 0 );
        } else if ( ! is_head ) {
            warning( "Naked symbol" );
            // printf( "[!] Symbol [%s]\n", o->data.s );
            eval = o;
        }
    }

    return eval;
}

object *eval_object( object *o ) {
    return _eval_object( o, false );
}

object *eval( object *o ) {
    printd( "Evaluating object" );
    o = eval_object( o );
    printd( "Finished evaluating object" );
    return o;
}
