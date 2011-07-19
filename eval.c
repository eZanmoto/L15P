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

int length( list *l ) {
    return is_null( l->cdr ) ? 0 : 1 + length( l->cdr );
}

int num_args( list *l ) {
    return length( l ) - 1;
}

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
            o = quote( o );
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
