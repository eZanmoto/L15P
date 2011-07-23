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

#include "types.h"
#include "read.h"
#include "eval.h"
#include "print.h"

/*
 * Outputs the splash text for L15P
 */
void output_splash() {
    printf( "L15P  Copyright (C) 2011  Sean Kelleher\n" );
    printf( "This program comes with ABSOLUTELY NO WARRANTY.\n" );
    printf( "This is free software, and you are welcome to redistribute it\n" );
    printf( "under certain conditions.\n" );
    printf( "Type 'quit' to exit.\n" );
}

/*
 * The read-eval-print loop.
 */
void repl() {
    printf( "\n>>> " );
    print( eval( read( stdin ) ) );
    repl();
}

/*
 * Starts the read-eval-print loop.
 *
 * \return '0' on successful termination
 */
int main() {
    output_splash();
    init_types();
    init_eval();
    repl();
    return 0;
}
