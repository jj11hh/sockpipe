#include "sockpipe/log.h"
#include "sockpipe/blackhole.h"

void cleanbh(SP_Blackhole **bh){
    Destroy_SP_Blackhole(*bh);
}

int main () {
    log_info("sockpipe started.");

    SCOPE(SP_Blackhole) *blackhole = Create_SP_Blackhole();
    SP_Packet *packet = Create_SP_Packet(64);

    log_debug("send a packet into blackhole");
    SP_Interface_send((SP_Interface *)blackhole, packet);

    log_info("exiting.");
}
