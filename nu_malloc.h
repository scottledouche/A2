#include <string.h>

#define align4(x) (((((x)-1)>>2)<<2)+4)
#define BLOCK_SIZE 12

typedef struct s_block *t_block;

struct s_block {
  size_t size;
  t_block next;
  int free;
  char data[1];
};

void* nu_malloc(size_t);
void* nu_calloc(size_t, size_t);
void* nu_realloc(void*, size_t);
void nu_free(void*);
