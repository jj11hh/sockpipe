#ifndef SOCKPIPE_INTERFACE
#define SOCKPIPE_INTERFACE

#include "sockpipe/object.h"
#include "sockpipe/packet.h"
#include "sockpipe/nullable.h"

#define TYPE_SP_Interface 2

/* Pure Virtual Class */

struct _SP_Interface;

DEF_META(SP_Interface) {
    type_t type;

    // VTable
    bool (* can_send) (struct _SP_Interface *self);
    bool (* can_recv) (struct _SP_Interface *self);

    int (* send) (struct _SP_Interface *self, SP_Packet *packet);
    Nullable_SP_Packet (* recv) (struct _SP_Interface *self);

    void (* del) (struct _SP_Interface *self);
};

typedef struct _SP_Interface {
    META(SP_Interface);

    
} SP_Interface;

DEF_NULLABLE(SP_Interface)

static inline void SP_Interface_init(SP_Interface *self){} // == 0

VIRTUAL static inline void SP_Interface_del(SP_Interface *self){
    INVOKE_VIRTUAL(self, del);    
}

static inline bool SP_Interface_can_send (SP_Interface *self){
    return INVOKE_VIRTUAL(self, can_send);
}

static inline bool SP_Interface_can_recv (SP_Interface *self){
    return INVOKE_VIRTUAL(self, can_recv);
}

static inline int SP_Interface_send (SP_Interface *self, SP_Packet *packet){
    return INVOKE_VIRTUAL(self, send, packet);
}

static inline Nullable_SP_Packet SP_Interface_recv (SP_Interface *self){
    return INVOKE_VIRTUAL(self, recv);
}

#endif //SOCKPIPE_INTERFACE
