#include <stdio.h>
#include <string.h>

#include "read.h"
#include "eval.h"

bool car_is( list *l, char *string ) {
    return strcmp( l->car, string ) == 0;
}

list *eval( list *l ) {
    list *eval;

    if ( car_is( l, "quote" ) ) {
        eval = l->cdr;
    } else if ( car_is( l, "cdr" ) ) {
        eval = l->cdr->cdr;
    }

    return eval;
}
