#include <stdio.h>

#include "types.h"
#include "read.h"
#include "print.h"
#include "debug.h"

void print_object( object *o );

void print_list( list *l ) {
    printf( "(" );
    if ( ! is_null( l ) ) {
        printd( "Printing regular list" );
        print_object( l->car );
        print_list( l->cdr );
    } else {
        printd( "Printing empty list" );
    }
    printf( ")" );
}

void print_object( object *o ) {
    if ( o->type == SYMBOL ) {
        printd( "Printing symbol" );
        printf( "%s", o->data.s );
    } else {
        printd( "Printing list" );
        print_list( o->data.l );
    }
}

void print( object *o ) {
    printd( "Printing object" );
    print_object( o );
    printd( "Finished printing object" );
}
