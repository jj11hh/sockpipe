#include <string.h>
#include "sockpipe/blackhole.h"

static bool can_send (SP_Interface *self);
static bool can_recv (SP_Interface *self);

static int send (SP_Interface *self, SP_Packet *packet);
static Nullable_SP_Packet recv (SP_Interface *self);

static void del (SP_Interface *self);

static DEF_META(SP_Interface) meta
    = { 
        .type = TYPE_SP_Blackhole,
        .can_send = &can_send,
        .can_recv = &can_recv,
        .send = &send,
        .recv = &recv,
        .del = &del
      };

void SP_Blackhole_init(SP_Blackhole *self){
    self->meta = (DEF_META(SP_Blackhole) *)&meta;

    SP_Interface_init((SP_Interface *)self); // Call parent init
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

static void del (SP_Interface *self) {(void)self;}
