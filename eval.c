#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "read.h"
#include "eval.h"
#include "util.h"

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
    return is_null( l->cdr ) ? 0 : 1 + num_args( l->cdr );
}

object *quote( object *o ) {
    object *result;
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
        printf( "*** Read list" );
        if ( is_null( o->data.l ) ) {
            printf( "*** Read empty list" );
            eval = o;
        } else if ( is_function( o->data.l, "quote" ) ) {
            o = quote( o );
        }
    } else {
        if ( streq( o->data.s, "quit" ) ) {
            printf( "\nGoodbye." );
            exit( 0 );
        }
        eval = o;
    }

    return eval;
}

object *eval( object *o ) {
    return eval_object( o );
}
