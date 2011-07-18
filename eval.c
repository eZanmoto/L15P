#include <stdio.h>
#include <string.h>

#include "types.h"
#include "read.h"
#include "eval.h"
#include "util.h"

/*
 * Returns 'true' if the list 'l' is the function 'function'.
 * \param l checks if this function is 'function'
 * \param function checks if 'l' is this function
 * \return 'true' if the list 'l' is the function 'function'
 */
bool is_function( list *l, symbol function ) {
    return strcmp( l->car, function ) == 0;
}

/*
 * Evaluates the list 'l'
 * \param l the list to evaluate
 * \return the value of 'l'
 */
list *eval( list *l ) {
    list *eval = l;

    if ( is_function( l, "quote" ) ) {
        if ( is_null( eval = l->cdr ) )
            error( "'quote' expects exactly one argument" );
    } else {
        printf( "*** Didn't get 'quote'" );
    }

    return eval;
}
