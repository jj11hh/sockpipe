#include <string.h>
#include "sockpipe/blackhole.h"


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

static DEF_META(SP_BlackholeIF) ifmeta
    = { 
        .type = TYPE_SP_BlackholeIF,
        .can_send = &can_send,
        .can_recv = &can_recv,
        .send = &send,
        .recv = &recv,
        .del = &ifdel,
        .set_notify = &set_notify
      };

void SP_BlackholeIF_init(SP_BlackholeIF *self){
    self->meta = &ifmeta;
    self->cb_notify = NULL;
    self->userdata = NULL;
}

static bool can_send (SP_Interface *self) {
    (void)self;
    return true;    
}

static bool can_recv (SP_Interface *self) {
    (void)self;
    // Block forever
    return false;
}

static void set_notify (SP_Interface *self, cb_notify_t notify, void *data)
{
    ((SP_BlackholeIF *)self)->cb_notify = notify;
    ((SP_BlackholeIF *)self)->userdata = data;
}

static int send (SP_Interface *self, SP_Packet *packet){
    log_debug("SP_Blackhole: droped %d bytes", packet->data_size);
    SP_Packet_downcount(packet); // Release it
    SP_BlackholeIF *iface = (SP_BlackholeIF*)self;

    if (iface->cb_notify != NULL){
        iface->cb_notify(iface->userdata);
    }

    return 0; // No Error
}

static Nullable_SP_Packet recv (SP_Interface *self){
    (void) self;
    return (Nullable_SP_Packet) { NULL };
}

static void ifdel (SP_Interface *self) {(void)self;}

static int get_n_interface(SP_Joint *self);
static Nullable_SP_Interface get_interface(SP_Joint *self, int index);
static void joint_del(SP_Joint *self);

static DEF_META(SP_Blackhole) joint_meta = {
    .type = TYPE_SP_Blackhole,
    .get_n_interface = &get_n_interface,
    .get_interface = &get_interface,
    .del = &joint_del
};

void SP_Blackhole_init(SP_Blackhole *self){
    self->meta = &joint_meta;
    SP_BlackholeIF_init(&self->interface);
}

static int get_n_interface(SP_Joint *self){
    (void) self;
    return 1;
}

static Nullable_SP_Interface
get_interface(SP_Joint *self, int index){
    if (index == 0){
        return (Nullable_SP_Interface) 
               {(SP_Interface *)&(((SP_Blackhole *)self)->interface)};
    }
    else {
        return (Nullable_SP_Interface) {NULL};
    }
}

static void joint_del(SP_Joint *joint) {
    SP_Blackhole *self = (SP_Blackhole *) joint;
    self->interface.meta->del((SP_Interface *)&self->interface);
}
