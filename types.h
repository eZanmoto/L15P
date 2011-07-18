typedef char bool;

#define true    1
#define false   0

#define MAX_SYMBOL_LEN  128

typedef char *symbol;

symbol new_symbol();

typedef struct list {
    symbol car;
    struct list *cdr;
} list;

list *new_list();
