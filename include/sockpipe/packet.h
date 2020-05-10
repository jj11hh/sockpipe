#ifndef SOCKPIPE_PACKET
#define SOCKPIPE_PACKET

#include "sockpipe/object.h"
#include "sockpipe/nullable.h"
#include <stdint.h>
#include <stdbool.h>

#define MAX_TAGS 256

DEF_META(SP_Packet) {
    type_t type;
    
    int global_count;
};

typedef struct {
    META(SP_Packet);

    int rc;                 // reference count

    uint8_t version;        // 4 for IPV4, 6 for IPV6
    uint16_t protocal;      // Protocol
    uint8_t ttl;            // Time to Live
    uint8_t s_addr [16];    // Source address
    uint8_t d_addr [16];    // Dest address
    uint16_t s_port;        // Source Port
    uint16_t d_port;        // Dest Port

    bool tags[MAX_TAGS];    // User Defined Tag

    size_t data_size;       // Data Size

    uint8_t data[0];
} SP_Packet;

DEF_NULLABLE(SP_Packet)

void SP_Packet_init(SP_Packet *self, size_t data_size);
void SP_Packet_del(SP_Packet *self);

SP_Packet * Create_SP_Packet(size_t data_size);
void Destroy_SP_Packet(SP_Packet *);

static inline void SP_Packet_upcount(SP_Packet *self){
    self->rc ++;    
}

static inline void SP_Packet_downcount(SP_Packet *self){
    self->rc --;

    if (self->rc <= 0){
        Destroy_SP_Packet(self);
    }
}

static inline void SP_Packet_setTag(SP_Packet *self, int tag) {
    self->tags[tag] = true;    
}

static inline bool SP_Packet_checkTag(SP_Packet *self, int tag) {
    return self->tags[tag];
}

GEN_Cleanup(SP_Packet)

#endif //SOCKPIPE_PACKET
