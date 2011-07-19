#include <stdio.h>

#include "types.h"
#include "read.h"
#include "print.h"

void print_object( object *o );

void print_list( list *l ) {
    printf( "(" );
    if ( ! is_null( l ) ) {
        print_object( l->car );
        print_list( l->cdr );
    }
    printf( ")" );
}

void print_object( object *o ) {
    if ( o->type == SYMBOL ) {
        //printf( "*** Printing symbol" );
        printf( "%s", o->data.s );
    } else {
        //printf( "*** Printing list" );
        print_object( o );
        print_list( o->data.l );
    }
}

void print( object *l ) {
    print_object( l );
}
