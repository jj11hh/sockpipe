#ifndef SOCKPIPE_BLACKHOLE
#define SOCKPIPE_BLACKHOLE

#include "sockpipe/joint.h"
#include <string.h>


/* interface for Blackhole */
#define TYPE_SP_BlackholeIF 4

DEF_META(SP_BlackholeIF) {
    META_SP_Interface
};

typedef struct _SP_BlackholeIF {
    META(SP_BlackholeIF);
} SP_BlackholeIF;

DEF_NULLABLE(SP_BlackholeIF)

void SP_BlackholeIF_init(SP_BlackholeIF *self);

/* Blackhole joint */
#define TYPE_SP_Blackhole 5

DEF_META(SP_Blackhole) {
    META_SP_Joint
};

typedef struct _SP_Blackhole {
    META(SP_Blackhole);
    SP_BlackholeIF interface;
} SP_Blackhole;

DEF_NULLABLE(SP_Blackhole)

void SP_Blackhole_init(SP_Blackhole * self);

GEN_Virtual(void, SP_Joint, SP_Blackhole, del)
GEN_Virtual(int, SP_Joint, SP_Blackhole, get_n_interface)
GEN_Virtual(Nullable_SP_Interface, SP_Joint, SP_Blackhole, get_interface, int, index)

GEN_Create(SP_Blackhole)
GEN_Destroy(SP_Blackhole)
GEN_Cleanup(SP_Blackhole)

#endif //SOCKPIPE_BLACKHOLE
