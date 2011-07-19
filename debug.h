#define debug       10
#define printd(s)   if ( debug ) { printf( "*** [%s] %s\n", __FILE__, s ); }
#define output(l,s) \
    if ( debug >= l ) { printf( "*** [%s] %s\n", __FILE__, s ); }
