#ifndef SOCKPIPE_ALLOC
#define SOCKPIPE_ALLOC

#include <stddef.h>
#include "sockpipe/log.h"
#include <assert.h>
#include <stdlib.h>

#ifndef NDEBUG
#define SP_Alloc(size) ({\
    size_t _size = (size_t) size;\
    void *ptr = malloc(_size);\
    assert(ptr != NULL);\
    log_debug("allocate %zu bytes on %p", _size, ptr);\
    ptr;\
})

#define SP_Free(ptr) do{\
    void *_ptr = (void *) ptr;\
    log_debug("free pointer %p", _ptr);\
    free(_ptr);\
}while(0)

#define SP_Calloc(nmemb, size) ({\
    size_t _nmemb = (size_t) nmemb;\
    size_t _size = (size_t) size;\
    void *ptr = calloc(_nmemb, _size);\
    assert(ptr);\
    log_debug("allocate %zux%zu bytes on %p",\
        _nmemb, _size, ptr);\
    ptr;\
})

#define SP_Realloc(ptr, size) ({\
    void *_ptr = (void *) ptr;\
    size_t _size = (size_t) size;\
    void *new_ptr = realloc(_ptr, _size);\
    log_debug("re-allocate %zu bytes from %p to %p",\
        _size, _ptr, new_ptr);\
    new_ptr;\
})
#else
#define SP_Alloc malloc
#define SP_Calloc calloc
#define SP_Free free
#define SP_Realloc realloc
#endif

#endif //SOCKPIPE_ALLOC
