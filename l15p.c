#include <stdio.h>

#include "types.h"
#include "read.h"
#include "eval.h"
#include "print.h"

/*
 * Outputs the splash text for L15P
 */
void output_splash() {
    printf( "L15P\n" );
    printf( "An interpretive LISP environment.\n" );
    printf( "Written by Sean Kelleher.\n" );
    printf( "ctrl+C to Exit\n" );
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
    repl();
    return 0;
}
