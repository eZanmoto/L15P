#include <stdio.h>
#include <stdlib.h>

void error( char *message ) {
    fprintf( stderr, "[!] Error: %s\n", message );
}

void fatal( char *message ) {
    fprintf( stderr, "!!! Fatal: %s\n", message );
    exit( 1 );
}
