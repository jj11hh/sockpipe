#ifndef SOCKPIPE_INTERFACE
#define SOCKPIPE_INTERFACE

#include "sockpipe/object.h"
#include "sockpipe/packet.h"
#include "sockpipe/nullable.h"

#define TYPE_SP_Interface 2

/* Pure Virtual Class */

struct _SP_Interface;

#define META_SP_Interface\
    type_t type;\
    bool (* can_send) (struct _SP_Interface *self);\
    bool (* can_recv) (struct _SP_Interface *self);\
    int (* send) (struct _SP_Interface *self, SP_Packet *packet);\
    Nullable_SP_Packet (* recv) (struct _SP_Interface *self);\
    void (* del) (struct _SP_Interface *self);


DEF_META(SP_Interface) { META_SP_Interface };

typedef struct _SP_Interface {
    META(SP_Interface);
} SP_Interface;

DEF_NULLABLE(SP_Interface)

GEN_Virtual(void, SP_Interface, SP_Interface, del)
GEN_Virtual(bool, SP_Interface, SP_Interface, can_send)
GEN_Virtual(bool, SP_Interface, SP_Interface, can_recv)
GEN_Virtual(int, SP_Interface, SP_Interface, send, SP_Packet *, packet)
GEN_Virtual(Nullable_SP_Packet, SP_Interface, SP_Interface, recv)

#endif //SOCKPIPE_INTERFACE
