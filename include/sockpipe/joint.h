#ifndef SOCKPIPE_JOINT
#define SOCKPIPE_JOINT

#include "sockpipe/interface.h"

#define TYPE_SP_Joint 3

/* Pure Virtual Class */

struct _SP_Joint;

DEF_META(SP_Joint) {
    type_t type;

    // VTable
    int (* get_n_interface)(struct _SP_Joint *self);
    Nullable_SP_Interface (* get_interface)(struct _SP_Joint *self, int index);

    void (* del) (struct _SP_Joint *self);
};

typedef struct _SP_Joint {
    META(SP_Joint);

} SP_Joint;

DEF_NULLABLE(SP_Joint);

static inline void SP_Joint_init(SP_Joint *self) {(void)self;} // == 0
static inline void SP_Joint_del (SP_Joint *self) {
    INVOKE_VIRTUAL(self, del); 
}

static inline int SP_Joint_get_n_interface(SP_Joint *self) {
    return INVOKE_VIRTUAL(self, get_n_interface);    
}

static inline Nullable_SP_Interface SP_Joint_get_interface(SP_Joint *self, int index) {
    return INVOKE_VIRTUAL(self, get_interface, index);
}

#endif
