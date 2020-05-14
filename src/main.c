#include "sockpipe/log.h"
#include "sockpipe/application.h"
#include "sockpipe/blackhole.h"

#include <stdio.h>

int main () {
    log_info("Starting sockpipe");

#define TYPE_OF_app SP_Application
#define TYPE_OF_blackhole SP_Blackhole
#define TYPE_OF_joints SP_List

    SP_Blackhole *blackhole = Create_SP_Blackhole();
    SP_Blackhole *blackhole1 = Create_SP_Blackhole();
    SP_Application *app = Create_SP_Application();
    SP_List *joints = Create_SP_List();

    INVOKE(joints, append, (void *) blackhole);
    INVOKE(joints, append, (void *) blackhole1);
    INVOKE(app, register, joints);

    printf("blackhole is %p\n", (void*)blackhole);
    printf("blackhole1 is %p\n", (void*)blackhole1);
    for (int i = 0; i < app->n_joint; i ++){
        printf("joints[%d] = %p\n", i, (void*)app->joints[i]);
    }

    printf("id of blackhole is %d\n", INVOKE(app, find_joint_id, (SP_Joint*)blackhole));
    printf("id of blackhole1 is %d\n", INVOKE(app, find_joint_id, (SP_Joint*)blackhole1));

    DELETE(joints);
    DELETE(app);
}
