#include "sockpipe/application.h"
#include "sockpipe/nullable.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

void SP_Application_init(SP_Application *app){
    //app->message_queue = Create_SP_List();
    app->event_loop = uv_default_loop();
    app->joints = NULL;
    app->n_joint = 0;
    app->ifs = NULL;
    app->n_if = 0;
    app->connections = NULL;
    //app->on_message = NULL;
    //app->on_message_data = NULL;
}

void SP_Application_del(SP_Application *app){
    //Destroy_SP_List(app->message_queue);
    //app->message_queue = NULL;
    
    for (int i = 0; i < app->n_joint; i ++)
        Destroy_SP_Joint(app->joints[i]);

    SP_Free(app->joints);
    app->joints = NULL;
    SP_Free(app->ifs);
    app->ifs = NULL;
    SP_Free(app->connections);
    app->connections = NULL;
    app->n_joint = 0;
    app->n_if = 0;
    //app->on_message = NULL;
    //app->on_message_data = NULL;
}

static int compar (const void *a, const void *b){
    return a > b ? 1 : -1;
}

static int binsearch(const void **base, int len, const void *ele) {
    int first, last, middle;

    first = 0;
    last = len - 1;
    middle = last / 2;

    while (first <= last){
        if (base[middle] < ele)
            first = middle + 1;
        else if (base[middle] == ele) {
            return middle;
        }
        else
            last = middle - 1;

        middle = (first + last)/2;
    }

    return -1; // not found
}

void SP_Application_register(SP_Application *app, const SP_List *list) {
    int n_joint = list->size;
    SP_ListNode *node = list->head;

    int n_if = 0;
    for (int i = 0; i < n_joint; i ++){
        n_if += SP_Joint_get_n_interface((SP_Joint *)node->data);
        node = node->next;
    }

    app->n_if = n_if;
    app->n_joint = n_joint;
    app->joints = (SP_Joint **)SP_Calloc(n_joint, sizeof(SP_Joint *));
    app->ifs = (SP_Interface **)SP_Calloc(n_if, sizeof(SP_Interface *));

    app->connections = SP_Calloc(n_if, sizeof(int));
    memset(app->connections, -1, n_if);

    node = list->head; 
    int p_if = 0;
    for (int i = 0; i < n_joint; i ++){
        app->joints[i] = (SP_Joint *) node->data;
        int n = SP_Joint_get_n_interface((SP_Joint *)node->data);
        for (int j = 0; j < n; j ++){
            app->ifs[p_if] = SP_unwrap(SP_Joint_get_interface((SP_Joint*) node->data, j));
            p_if ++;
        }
        node = node->next;
    }

    qsort(app->joints, n_joint, sizeof(void *), &compar);
    qsort(app->ifs, n_if, sizeof(void *), &compar);
}

SP_Joint * SP_Application_get_joint(SP_Application *app, int id){
    assert(id >= 0 && id < app->n_joint);
    return app->joints[id];
}

SP_Interface * SP_Application_get_iface(SP_Application *app, int id){
    assert(id >= 0 && id < app->n_if);
    return app->ifs[id];
}

int SP_Application_find_joint_id(SP_Application *app, const SP_Joint *joint){
    return binsearch((void **)app->joints, app->n_joint, (const void *)joint);
}

int SP_Application_find_iface_id(SP_Application *app, const SP_Interface *iface){
    return binsearch((void **)app->ifs, app->n_if, (const void *)iface);
}

void SP_Application_connect(SP_Application *app, int if1, int if2){
    assert(if1 >= 0 && if1 < app->n_if);
    assert(if2 >= 0 && if2 < app->n_if);

    app->connections[if1] = if2;
    app->connections[if2] = if1;
}

void SP_Application_disconnect(SP_Application *app, int if1, int if2){
    assert(if1 >= 0 && if1 < app->n_if);
    assert(if2 >= 0 && if2 < app->n_if);

    app->connections[if1] = -1;
    app->connections[if2] = -1;
}

int SP_Application_get_connected(SP_Application *app, int iface){
    assert(iface >= 0 && iface < app->n_if);

    return app->connections[iface];
}

bool SP_Application_is_connected(SP_Application *app, int if1, int if2){
    assert(if1 >= 0 && if1 < app->n_if);
    assert(if2 >= 0 && if2 < app->n_if);

    return app->connections[if1] == if2;
}

void SP_Application_run(SP_Application *app){
    
}
