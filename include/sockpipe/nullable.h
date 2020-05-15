#ifndef SOCKPIPE_NULLABLE
#define SOCKPIPE_NULLABLE

#include <assert.h>

#define _DEF_NULLABLE(T) \
typedef struct _Nullable_ ## T {\
    T * ptr;\
} Nullable_ ## T;

#define DEF_NULLABLE(T) _DEF_NULLABLE(T)

#define SP_isNull(nullable) ((nullable).ptr == (void *)0)

#define SP_unwrap(_nullable) ({\
    void *ptr = (void *) (_nullable).ptr;\
    assert(ptr != NULL);\
    ptr;\
})

#endif //SOCKPIPE_NULLABLE
