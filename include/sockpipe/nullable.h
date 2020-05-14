#ifndef SOCKPIPE_NULLABLE
#define SOCKPIPE_NULLABLE

#include <assert.h>

#define _DEF_NULLABLE(T) \
typedef struct _Nullable_ ## T {\
    T * ptr;\
} Nullable_ ## T;

#define DEF_NULLABLE(T) _DEF_NULLABLE(T)

#define SP_isNull(nullable) ((nullable).ptr == (void *)0)

#define SP_unwrap(nullable) ({\
    assert(! SP_isNull(nullable));\
    (nullable).ptr;\
})\

#endif //SOCKPIPE_NULLABLE
