#include "sockpipe/object.h"

static DEF_META(SP_Object) meta;

void SP_Object_init (SP_Object *self) {
    self->meta = &meta;

    /* do initialize stuffs */
}

void SP_Object_del (SP_Object *self) {
    /* do clean up */
}
