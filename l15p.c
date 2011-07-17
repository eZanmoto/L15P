#include <stdio.h>
#include "read.h"
#include "print.h"

void repl() {
    printf( ">>> " );
    print( read( stdin ) );
    repl();
}

int main() {
    init();
    repl();
    return 0;
}
