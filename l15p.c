#include <stdio.h>
#include "read.h"
#include "print.h"

void output_splash() {
    printf( "L15P\n" );
    printf( "An interpretive LISP environment.\n" );
    printf( "Written by Sean Kelleher.\n" );
    printf( "ctrl+C to Exit\n" );
}

void repl() {
    printf( "\n>>> " );
    print( read( stdin ) );
    repl();
}

int main() {
    output_splash();
    init();
    repl();
    return 0;
}
