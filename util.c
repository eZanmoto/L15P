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

#include "types.h"

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

int length( list *l ) {
    return is_null( l ) ? 0 : 1 + length( l->cdr );
}

object *head( object *o ) {
    return o->data.l->car;
}

object *second( object *o ) {
    return o->data.l->cdr->car;
}

object *third( object *o ) {
    return o->data.l->cdr->cdr->car;
}
