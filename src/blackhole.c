#include <string.h>
#include "sockpipe/blackhole.h"

static bool can_send (SP_Interface *self);
static bool can_recv (SP_Interface *self);
static int send (SP_Interface *self, SP_Packet *packet);
static Nullable_SP_Packet recv (SP_Interface *self);
static void ifdel (SP_Interface *self);

static DEF_META(SP_BlackholeIF) ifmeta
    = { 
        .type = TYPE_SP_BlackholeIF,
        .can_send = &can_send,
        .can_recv = &can_recv,
        .send = &send,
        .recv = &recv,
        .del = &ifdel
      };

void SP_BlackholeIF_init(SP_BlackholeIF *self){
    self->meta = &ifmeta;
}

static bool can_send (SP_Interface *self) {
    (void)self;
    return true;    
}

static bool can_recv (SP_Interface *self) {
    (void)self;
    return false;
}

static int send (SP_Interface *self, SP_Packet *packet){
    (void)self;
    SP_Packet_downcount(packet); // Release it

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
