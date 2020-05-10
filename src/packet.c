#include "sockpipe/packet.h"
#include <string.h>

#define TYPE_PACKET 1

static DEF_META(SP_Packet) meta = { TYPE_PACKET, 0 };

void SP_Packet_init (SP_Packet *self, size_t data_size){

    self->meta = &meta;
    self->rc        = 1;    // Reference Count
    self->version   = 4;    // IPv4 by default
    self->ttl       = 255;  // TTL
    self->data_size = data_size;

    // Left other fields zero
}

void SP_Packet_del (SP_Packet *self) {
    (void) self;
    // Nothing to do    
}

SP_Packet * Create_SP_Packet(size_t data_size){

    SP_Packet * self = SP_Alloc(sizeof(SP_Packet) + data_size);
    memset ((void *) self, 0, sizeof(SP_Packet) + data_size);

    SP_Packet_init(self, data_size);
    self->meta->global_count ++;

    return self;
}

void Destroy_SP_Packet(SP_Packet * self) {
    SP_Packet_del(self);
    self->meta->global_count --;
    SP_Free(self);
}
