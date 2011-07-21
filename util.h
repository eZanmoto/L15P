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

void error( char * );
void fatal( char * );

int length( list * );

object *head( object * );
object *second( object * );
object *third( object * );
