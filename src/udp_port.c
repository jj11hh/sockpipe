#include "sockpipe/udp_port.h"
#include "sockpipe/ip_protocol.h"
#include <stddef.h>
#include <string.h>
#include <assert.h>

#define CHECK(r, msg) \
do{if (r) {\
    log_error("%s: %s", msg, uv_strerror(r));\
    exit(1);\
}}while(0)

#define get_parent(child, parentType, fieldname)\
    (parentType *)((char *)(child) - ((char *)offsetof(parentType, fieldname)))

#define NOTIFY(joint)\
do {\
    if (joint->interface.cb_notify != NULL)\
        joint->interface.cb_notify(joint->interface.userdata);\
    }\
while(0)

/* Virtual functions */

static bool if_can_send(SP_Interface *self);
static bool if_can_recv(SP_Interface *self);
static int if_send(SP_Interface *self, SP_Packet *packet);
static Nullable_SP_Packet
if_recv (SP_Interface *self);
static void 
if_set_notify 
(SP_Interface *self, cb_notify_t notify, void *data);
static void if_del(SP_Interface *self);

static int jo_get_n_interface(SP_Joint *self);
static Nullable_SP_Interface
jo_get_interface(SP_Joint *self, int index);
static void jo_del(SP_Joint *self);

static DEF_META(SP_UdpPortIF) if_meta = {
    .can_send = if_can_send,
    .can_recv = if_can_recv,
    .send = if_send,
    .recv = if_recv,
    .set_notify = if_set_notify,
    .del = if_del
};

static DEF_META(SP_UdpPort) jo_meta = {
    .get_n_interface = jo_get_n_interface,
    .get_interface = jo_get_interface,
    .del = jo_del
};

void SP_UdpPortIF_init(SP_UdpPortIF *self){
    self->meta = &if_meta;
    self->cb_notify = NULL;
}


static void close_cb(uv_handle_t *handle){
    log_debug("handle closed: %p", (void *) handle);
}

static void 
on_recv (uv_udp_t* handle, 
         ssize_t nread, 
         const uv_buf_t* rcvbuf, 
         const struct sockaddr* addr, 
         unsigned flags);

static void 
on_alloc (uv_handle_t* client, size_t suggested_size, uv_buf_t* buf);

/* constructor */

void SP_UdpPort_init(SP_UdpPort *self, 
                     uv_loop_t *loop, 
                     const struct sockaddr *addr){
    int r;
    self->meta = &jo_meta;
    self->incoming = NULL;
    self->outgoing = NULL;
    SP_UdpPortIF_init(&self->interface);
    r = uv_udp_init(loop, &self->server);
    CHECK(r, "uv_udp_init");
    r = uv_udp_bind(&self->server, addr, 0);
    CHECK(r, "uv_udp_bind");
    r = uv_udp_recv_start(&self->server, &on_alloc, &on_recv);
    CHECK(r, "uv_udp_recv_start");
}

static void 
on_recv (uv_udp_t* handle, 
         ssize_t nread, 
         const uv_buf_t* rcvbuf, 
         const struct sockaddr* addr, 
         unsigned flags) {

    SP_Packet *packet = get_parent(rcvbuf->base, SP_Packet, data);
    SP_UdpPort *joint;
    (void)flags;

    if (nread <= 0)
        goto CLEANUP;

    joint = get_parent(handle, SP_UdpPort, server);

    if (joint->incoming != NULL){ // Last packet not recv
        log_warn("udp packet silently droped");
        goto CLEANUP;
    }

    struct sockaddr_storage d_addr;
    int addrlen = sizeof(d_addr);
    int r = uv_udp_getsockname(handle, (struct sockaddr *)&d_addr, &addrlen);
    CHECK(r, "uv_udp_getsockname");

    packet->data_size = (size_t) nread;
    packet->protocol = SP_UDP;
    if (addr->sa_family == AF_INET) { // IPv4
        const struct sockaddr_in *addr4 = (const struct sockaddr_in*) addr;
        struct sockaddr_in *d_addr4 = (struct sockaddr_in*) &d_addr;
        packet->version = 4;
        packet->s_port = ntohs((uint16_t)addr4->sin_port);
        packet->d_port = ntohs((uint16_t)d_addr4->sin_port);
        packet->s_addr4 = addr4->sin_addr.s_addr;
        packet->d_addr4 = d_addr4->sin_addr.s_addr;
    }
    else if (addr->sa_family == AF_INET6) { // IPv6
        const struct sockaddr_in6 *addr6 = (const struct sockaddr_in6*) addr;
        struct sockaddr_in6 *d_addr6 = (struct sockaddr_in6*) &d_addr;
        packet->version = 6;
        packet->s_port = ntohs((uint16_t)addr6->sin6_port);
        packet->d_port = ntohs((uint16_t)d_addr6->sin6_port);
        memcpy((void*)packet->s_addr6, (const void*)&addr6->sin6_addr, 2);
        memcpy((void*)packet->d_addr6, (const void*)&d_addr6->sin6_addr, 2);
    }
    else assert(0); // Will not reach here

    joint->incoming = packet;

    // Inform application joint is ready
    NOTIFY(joint);

    return;

    CLEANUP:
    SP_Packet_downcount(packet);
}

static void 
on_alloc (uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
    (void) handle;
    SP_Packet *packet = Create_SP_Packet(suggested_size);
    buf->base = (char*)packet->data;
    buf->len = suggested_size;
}

/* distructor */
static void jo_del(SP_Joint *_self){
    SP_UdpPort *self = (SP_UdpPort *) _self;
    SP_Interface_del((SP_Interface *)&self->interface);
    if (self->incoming != NULL)
        SP_Packet_downcount(self->incoming);

    if (self->outgoing != NULL)
        SP_Packet_downcount(self->outgoing);
    uv_close((uv_handle_t *) &self->server, &close_cb);
}

static bool if_can_send(SP_Interface* iface){
    SP_UdpPort* joint = get_parent(iface, SP_UdpPort, interface);
    return joint->outgoing == NULL;
}

static bool if_can_recv(SP_Interface* iface){
    SP_UdpPort* joint = get_parent(iface, SP_UdpPort, interface);
    return joint->incoming != NULL;
}

typedef struct _my_send_t {
    uv_udp_send_t req;
    SP_UdpPort *joint;
} my_send_t;

static void send_cb(uv_udp_send_t* req, int status);

static int if_send(SP_Interface* iface, SP_Packet* packet){
    if (! if_can_send(iface)){
        SP_Packet_downcount(packet);
        return -1;
    }
    if (packet->protocol != SP_UDP){
        SP_Packet_downcount(packet);
        return -1;
    }
    SP_UdpPort *joint = get_parent(iface, SP_UdpPort, interface);
    joint->outgoing = packet;

    int r;
    uv_buf_t buf = uv_buf_init((char *)packet->data, (uint32_t)packet->data_size);
    my_send_t *myreq = SP_Alloc(sizeof(my_send_t));
    myreq->joint = joint;

    struct sockaddr_storage addr;
    if (packet->version == 4) { // IPv4
        struct sockaddr_in *addr4 = (struct sockaddr_in*) &addr;
        addr4->sin_family = AF_INET;
        addr4->sin_port = htons(packet->d_port);
        addr4->sin_addr.s_addr = packet->d_addr4;
    }
    if (packet->version == 6) { // IPv6
        struct sockaddr_in6 *addr6 = (struct sockaddr_in6*) &addr;
        addr6->sin6_family = AF_INET6;
        addr6->sin6_port = htons(packet->d_port);
        memcpy((void*)&addr6->sin6_addr, (void*)packet->s_addr6, 2);
    }
    log_debug("UdpPort: sending packet %p, len = %d", buf.base, buf.len);
    r = uv_udp_send((uv_udp_send_t *)myreq,
        &joint->server,
        &buf,
        1,
        (const struct sockaddr *)&addr,
        &send_cb
    );

    CHECK(r, "uv_udp_send");

    return 0;
}

static void send_cb(uv_udp_send_t* req, int status){
    assert(req != NULL);
    (void) status;
    SP_UdpPort *joint = ((my_send_t*)req)->joint;
    SP_Packet_downcount(joint->outgoing);
    joint->outgoing = NULL;
    // Inform application packet was send
    NOTIFY(joint);
    log_debug("UdpPort: a packet send");

    // Handle was closed by uv itself, don't close again
    //uv_close((uv_handle_t *)req->handle, close_cb);
    SP_Free(req);
}


static Nullable_SP_Packet if_recv(SP_Interface *iface) {
    SP_UdpPort *joint = get_parent(iface, SP_UdpPort, interface);
    SP_Packet *packet = joint->incoming;
    joint->incoming = NULL;
    return (Nullable_SP_Packet) {packet}; // Maybe NULL
}

static void 
if_set_notify 
(SP_Interface *_self, cb_notify_t notify, void *data){
    SP_UdpPortIF *self = (SP_UdpPortIF *)_self;
    self->cb_notify = notify;
    self->userdata = data;
}

static void if_del(SP_Interface *_self){
    SP_UdpPortIF *self = (SP_UdpPortIF *)_self;
    self->cb_notify = NULL;
    self->userdata = NULL;
}

static int jo_get_n_interface(SP_Joint *self){
    (void) self;
    return 1;    
}

static Nullable_SP_Interface
jo_get_interface(SP_Joint *_self, int index){
    SP_UdpPort *self = (SP_UdpPort *)_self;
    if (index == 0)
        return (Nullable_SP_Interface){(SP_Interface*) &self->interface};
    else 
        return (Nullable_SP_Interface){(SP_Interface*) NULL};
}


