#define SYMBOL_LEN  128

typedef char bool;

#define true    1
#define false   0

typedef char *symbol;

typedef struct list {
    symbol car;
    struct list *cdr;
} list;

list *read( FILE *in );

void init();

extern list *empty_list;

bool is_null( list *l );
