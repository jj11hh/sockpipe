#ifndef SOCKPIPE_ECHO
#define SOCKPIPE_ECHO

#include "sockpipe/joint.h"
#include "sockpipe/packet.h"

DEF_META(SP_EchoIF) { META_SP_Interface };

typedef struct _SP_EchoIF {
    META(SP_EchoIF);
    cb_notify_t cb_notify;
    void *userdata;
} SP_EchoIF;

DEF_NULLABLE(SP_EchoIF);

void SP_EchoIF_init(SP_EchoIF *self);

DEF_META(SP_Echo) { META_SP_Joint };

typedef struct _SP_Echo {
    META(SP_Echo);
    SP_EchoIF interface;
    SP_Packet *pending;
} SP_Echo;

DEF_NULLABLE(SP_Echo);

void SP_Echo_init(SP_Echo *self);

GEN_Create(SP_Echo)
GEN_Virtual(void, SP_Joint, SP_Echo, del)
GEN_Destroy(SP_Echo)
GEN_Cleanup(SP_Echo)

#endif
