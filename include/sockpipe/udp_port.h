#ifndef SOCKPIPE_UDP_PORT
#define SOCKPIPE_UDP_PORT

#include "sockpipe/joint.h"
#include "sockpipe/packet.h"
#include "uv.h"

/* interface of UdpPort */
#define TYPE_SP_UdpPortIF 6

DEF_META(SP_UdpPortIF) { META_SP_Interface };

typedef struct _SP_UdpPortIF {
    META(SP_UdpPortIF);
    cb_notify_t cb_notify;
    void *userdata;
} SP_UdpPortIF;

DEF_NULLABLE(SP_UdpPortIF)

void SP_UdpPortIF_init(SP_UdpPortIF *self);

/* UDP Port Joint */
#define TYPE_SP_UdpPort 7

DEF_META(SP_UdpPort) { META_SP_Joint };

typedef struct _SP_UdpPort {
    META(SP_UdpPort);
    SP_UdpPortIF interface;
    uv_udp_t server;
    SP_Packet *pending;
    SP_Packet *incoming;
    SP_Packet *outgoing;
} SP_UdpPort;

DEF_NULLABLE(SP_UdpPort)

void SP_UdpPort_init
    (SP_UdpPort *self,
    uv_loop_t *loop,
    const struct sockaddr* addr);

GEN_Create(SP_UdpPort, uv_loop_t*, loop, const struct sockaddr*, addr)
GEN_Virtual(void, SP_Joint, SP_UdpPort, del)
GEN_Destroy(SP_UdpPort)
GEN_Cleanup(SP_UdpPort)

#endif //SOCKPIPE_UDP_PORT
