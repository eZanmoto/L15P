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

typedef char bool;

#define true    1
#define false   0

#define MAX_SYMBOL_LEN  128

typedef char *symbol;

symbol new_symbol();

extern symbol TRUE;

typedef enum { SYMBOL, LIST } object_type;

typedef struct object {
    object_type type;
    union {
        symbol s;
        struct list *l;
    } data;
} object;

typedef struct list {
    struct object *car;
    struct list *cdr;
} list;

list *new_list();

extern list *EMPTY_LIST;

void init_types();

object *new_object();

object *new_symbol_object();

object *new_list_object();

bool is_symbol( object *o );

bool is_list( object *o );

bool is_null( list *l );
