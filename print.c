#include <stdio.h>

#include "types.h"
#include "read.h"
#include "print.h"
#include "debug.h"

void print_object( object *o );

void print_list( list *l ) {
    printf( "(" );
    if ( ! is_null( l ) ) {
        output( 1, "Printing regular list" );
        print_object( l->car );
        print_list( l->cdr );
    } else {
        output( 1, "Printing empty list" );
    }
    printf( ")" );
}

void print_object( object *o ) {
    if ( o->type == SYMBOL ) {
        output( 2, "Printing symbol" );
        printf( "%s", o->data.s );
    } else {
        output( 2, "Printing list" );
        print_list( o->data.l );
    }
}

void print( object *o ) {
    output( 3, "Printing object" );
    print_object( o );
    output( 3, "Finished printing object" );
}
