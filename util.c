#include <stdio.h>
#include <stdlib.h>

void error( char *message ) {
    fprintf( stderr, "[!] Error: %s\n", message );
    exit( 1 );
}
