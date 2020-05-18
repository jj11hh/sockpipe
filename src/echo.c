#include <string.h>
#include "sockpipe/echo.h"

#define get_parent(child, parentType, fieldname)\
    (parentType *)((char *)(child) - offsetof(parentType, fieldname))

#define NOTIFY(joint)\
do {\
    if (joint->interface.cb_notify != NULL){\
        joint->interface.cb_notify(joint->interface.userdata);\
    }\
}\
while(0)

static bool can_send (SP_Interface *self);
static bool can_recv (SP_Interface *self);
static int send (SP_Interface *self, SP_Packet *packet);
static Nullable_SP_Packet recv (SP_Interface *self);
static void ifdel (SP_Interface *self);
static void set_notify (SP_Interface *self, cb_notify_t notify, void *data);

static DEF_META(SP_EchoIF) ifmeta
    = { 
        .type = 8,
        .can_send = &can_send,
        .can_recv = &can_recv,
        .send = &send,
        .recv = &recv,
        .del = &ifdel,
        .set_notify = &set_notify
      };

void SP_EchoIF_init(SP_EchoIF *self){
    self->meta = &ifmeta;
    self->cb_notify = NULL;
    self->userdata = NULL;
}

static bool can_send (SP_Interface *self) {
    SP_Echo *echo = get_parent(self, SP_Echo, interface);
    return echo->pending == NULL;
}

static bool can_recv (SP_Interface *self) {
    SP_Echo *echo = get_parent(self, SP_Echo, interface);
    return echo->pending != NULL;
}

static void set_notify (SP_Interface *self, cb_notify_t notify, void *data)
{
    ((SP_EchoIF *)self)->cb_notify = notify;
    ((SP_EchoIF *)self)->userdata = data;
}

static int send (SP_Interface *self, SP_Packet *packet){
    SP_Echo *echo = get_parent(self, SP_Echo, interface);
    assert(can_send(self));
    // swap packet addr and port
    uint8_t addr[16];
    uint16_t port;

    memcpy(addr, packet->s_addr6, 16);
    memcpy(packet->s_addr6, packet->d_addr6, 16);
    memcpy(packet->d_addr6, addr, 16);

    port = packet->s_port;
    packet->s_port = packet->d_port;
    packet->d_port = port;

    echo->pending = packet;

    NOTIFY(echo);

    return 0;
}

static Nullable_SP_Packet recv (SP_Interface *self){
    SP_Echo *echo = get_parent(self, SP_Echo, interface);
    assert(can_recv(self));
    SP_Packet *packet = echo->pending;
    echo->pending = NULL;

    NOTIFY(echo);
    return (Nullable_SP_Packet) { packet };
}

static void ifdel (SP_Interface *self) {(void)self;}

static int get_n_interface(SP_Joint *self);
static Nullable_SP_Interface get_interface(SP_Joint *self, int index);
static void joint_del(SP_Joint *self);

static DEF_META(SP_Echo) joint_meta = {
    .type = 9,
    .get_n_interface = &get_n_interface,
    .get_interface = &get_interface,
    .del = &joint_del
};

void SP_Echo_init(SP_Echo *self){
    self->meta = &joint_meta;
    SP_EchoIF_init(&self->interface);
}

static int get_n_interface(SP_Joint *self){
    (void) self;
    return 1;
}

static Nullable_SP_Interface
get_interface(SP_Joint *self, int index){
    if (index == 0){
        return (Nullable_SP_Interface) 
               {(SP_Interface *)&(((SP_Echo *)self)->interface)};
    }
    else {
        return (Nullable_SP_Interface) {NULL};
    }
}

static void joint_del(SP_Joint *joint) {
    SP_Echo *self = (SP_Echo *) joint;
    self->interface.meta->del((SP_Interface *)&self->interface);
}
