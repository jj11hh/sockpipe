#ifndef SOCKPIPE_ALLOC
#define SOCKPIPE_ALLOC

#include <stddef.h>
#include "sockpipe/log.h"
#include <assert.h>
#include <stdlib.h>

#define SP_Alloc(size) ({\
    void *ptr = malloc(size);\
    assert(ptr != NULL);\
    log_debug("allocate %zu bytes on %p", size, ptr);\
    ptr;\
})

#define SP_Free(ptr) do{\
    log_debug("free pointer %p", ptr);\
    free(ptr);\
}while(0)

#define SP_Calloc(nmemb, size) ({\
    void *ptr = calloc(nmemb, size);\
    assert(ptr);\
    log_debug("allocate %zux%zu bytes on %p",\
        nmemb, size, ptr);\
    ptr;\
})

#define SP_Realloc(ptr, size) ({\
    void *new_ptr = realloc(ptr, size);\
    log_debug("re-allocate %zu bytes from %p to %p",\
        size, ptr, new_ptr);\
    return new_ptr;\
})

#endif //SOCKPIPE_ALLOC
