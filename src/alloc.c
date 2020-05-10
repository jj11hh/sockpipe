#include "sockpipe/alloc.h"
#include "sockpipe/log.h"

#include <stdlib.h>
#include <assert.h>

void *SP_Alloc(size_t size){
    void *ptr = malloc(size);
    assert(ptr != NULL);

    log_debug("allocate %zu on %p", size, ptr);
    return ptr;
}

void *SP_Free(void *ptr){
    log_debug("free pointer %p", ptr);
    free(ptr);
}
