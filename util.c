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
#include "debug.h"

void warning( char *message ) {
    fprintf( stderr, "[!] Warning: %s\n", message );
}

void error( char *message ) {
    fprintf( stderr, "[!] Error: %s\n", message );
}

void fatal( char *message ) {
    fprintf( stderr, "!!! Fatal: %s\n", message );
    exit( 1 );
}

bool streq( char *a, char *b ) {
    return strcmp( a, b ) == 0;
}

list *copy_list( list * );

object *copy_object( object *o ) {
    object *copy = new_object();
    copy->type = o->type;

    output( 3, ">>>Copying object" );
    switch ( o->type ) {
    case SYMBOL:
        output( 4, ">>>>Copying symbol" );
        free( copy );
        copy = new_symbol_object();
        strcpy( copy->data.s, o->data.s );
        output( 4, "<<<<Copying symbol" );
        break;
    case LIST: case FUNCTION:
        output( 4, ">>>>Copying list/function" );
        output( 5, ">>>>>Copying sublist" );
        copy->data.l = copy_list( o->data.l );
        output( 5, "<<<<<Copying sublist" );
        output( 4, "<<<<Copying list/function" );
        break;
    case PARAMETER:
        output( 4, ">>>>Copying parameter" );
        copy->data.p = o->data.p;
        output( 4, "<<<<Copying parameter" );
        break;
    }
    output( 3, "<<<Copying object" );

    return copy;
}

bool is_atomic( object *o ) {
    return SYMBOL == o->type
        || ( LIST == o->type && is_null( o->data.l ) );
}

void verify_list( object *o ) {
    if ( o->type != LIST ) fatal( "Object is not a list" );
}

#include "print.h"

list *copy_list( list *l ) {
    list *copy;

    output( 3, ">>>Copying list" );
    if ( is_null( l ) ) {
        output( 4, "*** Copied empty list" );
        copy = EMPTY_LIST;
    } else {
        output( 4, ">>>>[l]Creating new list" );
        copy = new_list();
        output( 4, "<<<<[l]Creating new list" );
        output( 4, ">>>>[l]Copying subobject" );
        // print( l->car );
        output( 4, ">>>>[l]Copying subobject" );
        object *o = copy_object( l->car ); 
        output( 4, "<<<<[l]Copying subobject" );
        output( 4, ">>>>[l]Assigning subobject" );
        copy->car = o;
        output( 4, "<<<<[l]Assigning subobject" );
        output( 4, "<<<<[l]Copying subobject" );
        output( 4, ">>>>[l]Copying sublist" );
        copy->cdr = copy_list( l->cdr );
        output( 4, "<<<<[l]Copying sublist" );
    }
    output( 3, "<<<Copying list" );

    return copy;
}

int length( list *l ) {
    return is_null( l ) ? 0 : 1 + length( l->cdr );
}

object *first( object *o ) {
    verify_list( o );
    return o->data.l->car;
}

object *second( object *o ) {
    verify_list( o );
    return o->data.l->cdr->car;
}

object *third( object *o ) {
    verify_list( o );
    return o->data.l->cdr->cdr->car;
}
