typedef char bool;

#define true    1
#define false   0

#define MAX_SYMBOL_LEN  128

typedef char *symbol;

symbol new_symbol();

typedef enum { SYMBOL, LIST } object_type;

typedef struct object {
    object_type type;
    union {
        symbol s;
        struct list *l;
    } data;
} object;

typedef struct list {
    struct object *car;
    struct list *cdr;
} list;

list *new_list();

extern list *EMPTY_LIST;

void init_types();

object *new_object();

bool is_symbol( object *o );

bool is_list( object *o );

bool is_null( list *l );
