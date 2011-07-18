#include <stdio.h>
#include "types.h"
#include "read.h"
#include "print.h"

void print_list( list *l ) {
    printf( "(" );
    if ( ! is_null( l ) ) {
        printf( "%s", l->car );
        print_list( l->cdr );
    }
    printf( ")" );
}

void print( list *l ) {
    print_list( l );
}
