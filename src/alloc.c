#include "sockpipe/alloc.h"
#include "sockpipe/log.h"

#include <stdlib.h>
#include <assert.h>

void *SP_Alloc(size_t size){
    void *ptr = malloc(size);
    assert(ptr != NULL);

    log_debug("allocate %zu bytes on %p", size, ptr);
    return ptr;
}

void SP_Free(void *ptr){
    log_debug("free pointer %p", ptr);
    free(ptr);

    return;
}

void *SP_Calloc(size_t nmemb, size_t size){
    void *ptr = calloc(nmemb, size);
    assert(ptr);

    log_debug("allocate %zu x %zu byte on %p",
        nmemb, size, ptr);

    return ptr;
}

void *SP_Realloc(void *ptr, size_t size){
    void *new_ptr = realloc(ptr, size);
    log_debug("re-allocate %zu bytes from %p to %p",
        size, ptr, new_ptr);

    return new_ptr;
}
