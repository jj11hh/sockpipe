#ifndef SOCKPIPE_APPLICATION
#define SOCKPIPE_APPLICATION

#include "sockpipe/object.h"
#include "sockpipe/list.h"
#include "sockpipe/interface.h"
#include "sockpipe/joint.h"
#include "uv.h"
#include <stdint.h>
#include <stdbool.h>


/* sockpipe Application Class */

typedef struct _SP_Application {
    //SP_List *notify_queue;
    uv_loop_t *event_loop;
    SP_Joint **joints;
    int n_joint;
    SP_Interface **ifs;
    int n_if;
    int *connections;
} SP_Application;

void SP_Application_init(SP_Application *app);
void SP_Application_del(SP_Application *app);
void SP_Application_register(SP_Application *app, const SP_List *joints);
int SP_Application_find_joint_id(SP_Application *app, const SP_Joint *joint);
SP_Joint * SP_Application_get_joint(SP_Application *app, int id);
int SP_Application_find_iface_id(SP_Application *app, const SP_Interface *iface);
SP_Interface * SP_Application_get_iface(SP_Application *app, int id);
//void SP_Application_notify(SP_Application *app, const SP_Joint *joint);
void SP_Application_connect(SP_Application *app, int if1, int if2);
void SP_Application_disconnect(SP_Application *app, int if1, int if2);
int SP_Application_get_connected(SP_Application *app, int iface);
bool SP_Application_is_connected(SP_Application *app, int if1, int if2);

void SP_Application_run(SP_Application *app);

GEN_Create(SP_Application)
GEN_Destroy(SP_Application)
GEN_Cleanup(SP_Application)

#endif //SOCKPIPE_APPLICATION
