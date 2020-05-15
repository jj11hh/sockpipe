#include "sockpipe/log.h"
#include "sockpipe/application.h"
#include "sockpipe/blackhole.h"
#include "sockpipe/udp_port.h"
#include "sockpipe/ip_protocol.h"

#include <stdio.h>

typedef struct {
    uv_timer_t timer;
    SP_Interface *iface;
} my_timer_t;

static void cb_timer(uv_timer_t *handle){
    my_timer_t *timer = (my_timer_t *)handle;
    printf("HI, timer triggered!\n");
    SP_Packet *packet = Create_SP_Packet(6);
    packet->protocol = SP_UDP;
    packet->d_port = 7700;
    uint8_t addr[4] = {127, 0, 0, 1};
    packet->d_addr4 = *((uint32_t *)&addr);
    memcpy(packet->data, "Hello\n", 6);
    SP_Interface_send(timer->iface, packet);
}

int main () {
    log_info("Starting sockpipe");

#define TYPE_OF_app SP_Application
#define TYPE_OF_blackhole SP_Blackhole
#define TYPE_OF_udp_port SP_UdpPort
#define TYPE_OF_joints SP_List

    SCOPE_CREATE(app);
    SCOPE_CREATE(joints);

    my_timer_t timer;

    struct sockaddr_in addr;
    int r = uv_ip4_addr("127.0.0.1", 8800, &addr);
    assert(r == 0);
    CREATE(udp_port, app->event_loop, (struct sockaddr *)&addr);
    CREATE(blackhole);

    uv_timer_init(app->event_loop, (uv_timer_t *)&timer);

    INVOKE(joints, append, (void *) blackhole);
    INVOKE(joints, append, (void *) udp_port);
    INVOKE(app, register, joints);

    printf("blackhole is %p\n", (void*)blackhole);
    printf("udp_port is %p\n", (void*)udp_port);
    for (int i = 0; i < app->n_joint; i ++){
        printf("joints[%d] = %p\n", i, (void*)app->joints[i]);
    }

    printf("id of blackhole is %d\n", INVOKE(app, find_joint_id, (SP_Joint*)blackhole));
    printf("id of udp_port is %d\n", INVOKE(app, find_joint_id, (SP_Joint*)udp_port));

    INVOKE(app, connect, 0, 1);

#define TYPE_OF_packet SP_Packet
#define TYPE_OF_blackif SP_Interface
#define TYPE_OF_udp_if SP_Interface

    log_info("testing blackhole");
    SP_Packet *packet;
    SP_Interface *blackif = SP_unwrap(INVOKE(blackhole, get_interface, 0));
    SP_Interface *udp_if = SP_unwrap(INVOKE_VIRTUAL((SP_Joint*)udp_port, get_interface, 0));

    for (int i = 0; i < 10; i ++){
        packet = Create_SP_Packet(6);
        packet->protocol = SP_UDP;
        packet->d_port = 7700;
        uint8_t addr[4] = {127, 0, 0, 1};
        packet->d_addr4 = *((uint32_t *)&addr);
        memcpy(packet->data, "Hello\n", 6);
        INVOKE(blackif, send, packet);
    }

    log_info("running application");

    timer.iface = udp_if;
    uv_timer_start((uv_timer_t *)&timer, cb_timer, 0, 1000);
    INVOKE(app, run);
}
