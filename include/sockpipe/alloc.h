#ifndef SOCKPIPE_ALLOC
#define SOCKPIPE_ALLOC

#include <stddef.h>

void * SP_Alloc(size_t size);
void * SP_Calloc(size_t nmemn, size_t size);
void * SP_Realloc(void *ptr, size_t size);
void SP_Free(void *ptr);

#endif //SOCKPIPE_ALLOC
