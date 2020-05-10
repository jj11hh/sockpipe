#ifndef SOCKPIPE_OBJECT
#define SOCKPIPE_OBJECT

#include "sockpipe/alloc.h"
#include <string.h>

typedef int type_t;

#define VIRTUAL

#define INVOKE_VIRTUAL(O, M, ...) ((O)->meta->M((O), ##__VA_ARGS__))

#define _INVOKE_METHOD(O, T, M, ...) (T ## _ ## M((O), ##__VA_ARGS__))
#define INVOKE_METHOD(...) _INVOKE_METHOD(__VA_ARGS__)

#define _TYPE_OF(O) TYPE_OF_ ## O
#define TYPE_OF(O) _TYPE_OF(O)

#define INVOKE(O, ...) INVOKE_METHOD(O, TYPE_OF(O), __VA_ARGS__)

#define _DEF_METHOD(T, M) T ## _ ## M
#define DEF_METHOD(T, M) _DEF_METHOD(T, M)

#define _DEF_META(T) struct _Meta_ ## T
#define DEF_META(T) _DEF_META(T)

#define _GEN_Create(T, ...) \
static inline T * Create_ ## T (## __VA_ARGS__) {\
    T * self = (T *) SP_Alloc(sizeof(T));\
    memset(self, 0, sizeof(T));\
    INVOKE_METHOD(self, T, init, ##__VA_ARGS__);\
    return self;\
}

#define _GEN_Destroy(T) \
static inline void Destroy_ ## T (T * self) {\
    INVOKE_METHOD(self, T, del);\
    SP_Free(self);\
}

#define _GEN_Virtual(R, T, M, ...) \
static inline R T ## _ ## M (T * self, ##__VA_ARGS__) {\
    return INVOKE_VIRTUAL(self, M, ##__VA_ARGS__);\
}

#define _GEN_Cleanup(T)\
static inline void __Cleanup_ ## T ( T ** selfp) {\
    Destroy_ ## T(*selfp);\
}

#define _CLEANUP(T) __Cleanup_ ## T
#define CLEANUP(T) _CLEANUP(T)

#define META(T) DEF_META(T) * meta

#define GEN_Create(...) _GEN_Create(__VA_ARGS__)
#define GEN_Destroy(...) _GEN_Destroy(__VA_ARGS__)
#define GEN_Virtual(...) _GEN_Virtual(__VA_ARGS__)
#define GEN_Cleanup(...) _GEN_Cleanup(__VA_ARGS__)

#define SCOPE(T) __attribute__((cleanup (CLEANUP(T)))) T

/* Define type SP_Object */

#define TYPE_SP_Object 0

DEF_META(SP_Object) {
    type_t type; 
    
    /* Add virtual functions and static class variables here */
};

typedef struct _SP_Object {
    META(SP_Object);

} SP_Object;

void SP_Object_init(SP_Object *self);
void SP_Object_del(SP_Object *self);

GEN_Create(SP_Object)
GEN_Destroy(SP_Object)

/* For virtual functions, define a inline method that invoke the virtual one */

/*
 * GEN_Virtual(SP_Object, del);
 */

#endif
