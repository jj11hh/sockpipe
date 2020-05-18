#include "sockpipe/log.h"
#include "sockpipe/application.h"
#include "sockpipe/echo.h"
#include "sockpipe/udp_port.h"
#include "sockpipe/ip_protocol.h"

#include <stdio.h>

typedef struct {
    uv_timer_t timer;
    SP_Interface *iface;
} my_timer_t;

int main () {
    log_info("Starting sockpipe");

#define TYPE_OF_app SP_Application
#define TYPE_OF_echo SP_Echo
#define TYPE_OF_udp_port SP_UdpPort
#define TYPE_OF_joints SP_List

    SCOPE_CREATE(app);
    SCOPE_CREATE(joints);

    struct sockaddr_in addr;
    int r = uv_ip4_addr("127.0.0.1", 8800, &addr);
    CHECK(r, "uv_ip4_addr");

    CREATE(udp_port, app->event_loop, (struct sockaddr *)&addr);
    CREATE(echo);

    INVOKE(joints, append, (void *) echo);
    INVOKE(joints, append, (void *) udp_port);
    INVOKE(app, register, joints);

    INVOKE(app, connect, 0, 1);

    INVOKE(app, run);

    return 0;
}
