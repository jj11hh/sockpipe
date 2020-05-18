#include "sockpipe/application.h" 
#include "sockpipe/nullable.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

void SP_Application_init(SP_Application *app){
    app->notify_queue = Create_SP_List();
    app->event_loop = uv_default_loop();
    app->joints = NULL;
    app->n_joint = 0;
    app->ifs = NULL;
    app->n_if = 0;
    app->connections = NULL;
    app->inform_datas = NULL;
}

void SP_Application_del(SP_Application *app){
    Destroy_SP_List(app->notify_queue);
    app->notify_queue = NULL;
    
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
    SP_Free(app->inform_datas);
    app->inform_datas = NULL;
}

static int compar (const void *a, const void *b){
    return a == b ? 0 :
           a <  b ? 1 :
                   -1;
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

typedef struct _aync_update{
    uv_async_t async_handle;
    SP_Application *app;
} async_update;

static void close_cb(uv_handle_t *handle){
    SP_Free(handle);
    log_debug("handle %p was closed", (void*)handle);
}
static void cb_async_update(uv_async_t *handle){
    async_update *async = (async_update *)handle;
    SP_Application *app = async->app;

    while(app->notify_queue->size){
        void *ptr = SP_List_pop(app->notify_queue, 0);
        int id = (int)(intptr_t)ptr;
        int counterpart = app->connections[id];
        if (   SP_Interface_can_send(app->ifs[id])
            && SP_Interface_can_recv(app->ifs[counterpart])){
            log_debug("transfer packet from %d to %d", counterpart, id);
            SP_Packet *packet = SP_unwrap(SP_Interface_recv(app->ifs[counterpart]));
            SP_Interface_send(app->ifs[id], packet);
        }
        if (   SP_Interface_can_recv(app->ifs[id])
            && SP_Interface_can_send(app->ifs[counterpart])){
            log_debug("transfer packet from %d to %d", id, counterpart);
            SP_Packet *packet = SP_unwrap(SP_Interface_recv(app->ifs[id]));
            SP_Interface_send(app->ifs[counterpart], packet);
        }
    }

    log_debug("closing async handle %p", (void *)handle);
    uv_close((uv_handle_t *)handle, &close_cb);
}

static void cb_notify(void *data){
    SP_InformData *userdata = (SP_InformData *)data;
    SP_Application *app = userdata->app;

    log_debug("interface %d is ready", userdata->id);
    int counterpart = app->connections[userdata->id];
    if (counterpart != -1){
        // We will make the check of queue only happen once for a sequence of notifications
        // But a packet arrival event may make a interface notify more than once
        // If there is no element in the queue, we call the update function
        // and for other more notifications, just omit it
        if (app->notify_queue->size == 0) {
            async_update *handle = SP_Alloc(sizeof(async_update));
            log_debug("create async handle %p", (void *)handle);
            handle->app = app;
            uv_async_init(app->event_loop, (uv_async_t*) handle, &cb_async_update);
            uv_async_send((uv_async_t*)handle);
        }

        SP_List_append(app->notify_queue, (void *)(intptr_t)userdata->id);
    }
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
    app->connections = (int *)SP_Calloc(n_if, sizeof(int));
    memset(app->connections, -1, n_if * sizeof(int));
    app->inform_datas = (SP_InformData *)SP_Calloc(n_if, sizeof(SP_InformData));

    node = list->head; 
    int p_if = 0;
    for (int i = 0; i < n_joint; i ++){
        app->joints[i] = (SP_Joint *) node->data;
        int n = SP_Joint_get_n_interface((SP_Joint *)node->data);
        for (int j = 0; j < n; j ++){
            app->ifs[p_if] = SP_unwrap(SP_Joint_get_interface((SP_Joint*) node->data, j));
            log_debug("interface %p registered, id = %d", app->ifs[p_if], p_if);
            p_if ++;
        }
        node = node->next;
    }

    // Sort points to support binary search
    qsort(app->joints, (size_t)n_joint, sizeof(void *), &compar);
    qsort(app->ifs, (size_t)n_if, sizeof(void *), &compar);

    for (int i = 0; i < n_if; i ++){
        app->inform_datas[i].app = app;
        app->inform_datas[i].id = i;

        SP_Interface_set_notify(app->ifs[i], &cb_notify, (void *) &app->inform_datas[i]);
    }
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
    return binsearch((const void **)app->joints, app->n_joint, (const void *)joint);
}

int SP_Application_find_iface_id(SP_Application *app, const SP_Interface *iface){
    return binsearch((const void **)app->ifs, app->n_if, (const void *)iface);
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
    uv_run(app->event_loop, UV_RUN_DEFAULT);
}
