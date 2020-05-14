#ifndef SOCKPIPE_JOINT
#define SOCKPIPE_JOINT

#include "sockpipe/interface.h"

#define TYPE_SP_Joint 3

/* Pure Virtual Class */

struct _SP_Joint;

#define META_SP_Joint\
    type_t type;\
    int (* get_n_interface)(struct _SP_Joint *self);\
    Nullable_SP_Interface (* get_interface)(struct _SP_Joint *self, int index);\
    void (* del) (struct _SP_Joint *self);

DEF_META(SP_Joint) {
    META_SP_Joint
};

typedef struct _SP_Joint {
    META(SP_Joint);

} SP_Joint;

DEF_NULLABLE(SP_Joint);

GEN_Virtual(void, SP_Joint, SP_Joint, del)
GEN_Virtual(int, SP_Joint, SP_Joint, get_n_interface)
GEN_Virtual(Nullable_SP_Interface, SP_Joint, SP_Joint, get_interface, int, index)

GEN_Destroy(SP_Joint)

#endif
