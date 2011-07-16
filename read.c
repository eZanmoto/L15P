object *read_list( FILE *in ) {
    int c;
    object *car;
    object *cdr;
}

object *read( FILE *in ) {
    int c;
    object* result;

    eat_whitespace( in );
    c = getc( in );

    if ( '(' == c ) {
        result = read_list( in );
    } else {
        error( "expected list" );
    }

    return result;
}
