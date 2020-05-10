#ifndef SOCKPIPE_BLACKHOLE
#define SOCKPIPE_BLACKHOLE

#include "sockpipe/interface.h"
#include <string.h>

#define TYPE_SP_Blackhole 4

DEF_META(SP_Blackhole) {
    DEF_META(SP_Interface) interface;
};

typedef struct _SP_Blackhole {
    META(SP_Blackhole);
} SP_Blackhole;

DEF_NULLABLE(SP_Blackhole)

void SP_Blackhole_init(SP_Blackhole *self);

static inline SP_Blackhole * Create_SP_Blackhole () {
    SP_Blackhole * self = SP_Alloc(sizeof(SP_Blackhole));
    memset((void*) self, 0, sizeof(SP_Blackhole));
    SP_Blackhole_init(self);

    return self;
}

static inline void * Destroy_SP_Blackhole (SP_Blackhole * self) {
    SP_Interface_del((SP_Interface *)self);
    SP_Free(self);
}

GEN_Cleanup(SP_Blackhole)

#endif //SOCKPIPE_BLACKHOLE
