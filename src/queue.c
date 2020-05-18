#include "sockpipe/queue.h"

bool SP_Queue_enqueue(SP_Queue *queue, void * obj) {
    if (SP_Queue_is_full(queue))
        return false;

    queue->_queue[queue->tail ++] = obj;
    queue->size ++;
    
    if (queue->tail >= queue->max_size) {
        queue->tail = 0;    
    }

    return true;
}

bool SP_Queue_dequeue(SP_Queue *queue, void **obj) {
    if (SP_Queue_is_empty(queue))
        return false;
    
    queue->size --;
    *obj = queue->_queue[queue->head ++];

    if (queue->head >= queue->max_size) {
        queue->head = 0;    
    }

    return true;
}
