#ifndef SOCKPIPE_QUEUE
#define SOCKPIPE_QUEUE

#include "sockpipe/alloc.h"
#include "sockpipe/object.h"
#include <stdbool.h>

typedef struct _SP_Queue {
    int head;
    int tail;
    int max_size;
    int size;

    void * _queue[0];
} SP_Queue; 

static inline SP_Queue * Create_SP_Queue(int size) {
    
    SP_Queue * queue = (SP_Queue*) SP_Alloc(sizeof(SP_Queue) + size*sizeof(void *));
    queue->max_size = size;
    queue->head = 0;
    queue->tail = 0;
    queue->size = 0;

    return queue;
}

static inline void Destroy_SP_Queue(SP_Queue * queue){
    SP_Free(queue);    
}

GEN_Cleanup(SP_Queue)

static inline bool SP_Queue_is_full(SP_Queue *queue){
    return queue->max_size == queue->size;    
}

static inline bool SP_Queue_is_empty(SP_Queue *queue){
    return queue->size == 0;    
}

bool SP_Queue_enqueue(SP_Queue *queue, void *obj);
bool SP_Queue_dequeue(SP_Queue *queue, void **obj);

#endif //SOCKPIPE_QUEUE
