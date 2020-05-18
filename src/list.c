#include "sockpipe/list.h"
#include "assert.h"

static inline SP_ListNode * list_index(SP_List *list, int pos) {
    SP_ListNode *cur;
    if (pos <= list->size / 2){
        cur = list->head;
        for (int i = 0; i < pos; i ++) cur = cur->next;
    }
    else {
        cur = list->tail;
        for (int i = 0; i < list->size - pos - 1; i ++) cur = cur->prev;
    }
    return cur;
}

SP_ListNode * Create_SP_ListNode () {
    SP_ListNode * node = SP_Alloc(sizeof(SP_ListNode));
    SP_ListNode_init(node);

    return node;
}

void SP_ListNode_init(SP_ListNode *node) {
    *node = (SP_ListNode) {NULL, NULL, NULL};
}

void SP_ListNode_del(SP_ListNode *node){
    *node = (SP_ListNode) {NULL, NULL, NULL};
}

void Destroy_SP_ListNode(SP_ListNode *node){
    SP_ListNode_del(node);
    SP_Free(node);
}

SP_List * Create_SP_List() {
    SP_List * list = SP_Alloc(sizeof(SP_List));
    SP_List_init(list);
    return list;
}

void SP_List_init(SP_List *list) {
    *list = (SP_List) {0, NULL, NULL};
}

void SP_List_insert(SP_List *list, int pos, void *data) {
    assert(pos <= list->size);
    SP_ListNode *node = Create_SP_ListNode();
    node->data = data;

    if (list->size == 0) {
        list->size = 1;
        list->head = node;
        list->tail = node;

        return;
    }

    if (pos == 0) {
        list->size ++;
        list->head->prev = node;
        node->next = list->head;
        list->head = node;

        return;
    }

    if (pos == list->size) {
        list->size ++;
        list->tail->next = node;
        node->prev = list->tail;
        list->tail = node;

        return;
    }

    SP_ListNode * cur = list_index(list, pos);
    SP_ListNode * prev;

    prev = cur->prev;
    prev->next = node;
    node->prev = prev;
    cur->prev = node;
    node->next = cur;

    list->size ++;
}

void SP_List_append(SP_List *list, void *data){
    SP_ListNode *node = Create_SP_ListNode();
    node->data = data;

    if (list->size == 0){
        list->size = 1;
        list->head = node;
        list->tail = node;
        return;
    }

    list->size ++;
    list->tail->next = node;
    node->prev = list->tail;
    list->tail = node;
}

void SP_List_insert_before (SP_List *list, SP_ListNode *next, void *data) {
    SP_ListNode *node = Create_SP_ListNode();
    node->data = data;
    list->size ++;

    if (next == list->head) {
        list->head = node;
    }

    if (next->prev == NULL) {
        next->prev = node;
        node->next = next;
    }
    else {
        SP_ListNode *prev = next->prev;
        prev->next = node; node->prev = prev;
        next->prev = node; node->next = next;
    }
}

void SP_List_append_after (SP_List *list, SP_ListNode *prev, void *data) {
    SP_ListNode *node = Create_SP_ListNode();
    node->data = data;
    list->size ++;

    if (prev == list->tail){
        list->tail = node;
    }

    if (prev->next == NULL) {
        prev->next = node;
        node->prev = prev;
    }
    else {
        SP_ListNode *next = prev->next;
        next->prev = node; node->next = next;
        prev->next = node; node->prev = prev;
    }
}

void * SP_List_replace (SP_List *list, int pos, void *replacement){
    assert(pos < list->size && pos >= 0);
    void *replaced;
    SP_ListNode * cur = list_index(list, pos);
    replaced = cur->data;
    cur->data = replacement;

    return replaced;
}


void * SP_List_pop (SP_List *list, int pos) {
    assert(pos < list->size && pos >= 0);
    void *poped;
    list->size --;
    SP_ListNode *cur = list_index(list, pos);
    poped = cur->data;

    if (list->head == cur)
        list->head = cur->next;

    if (list->tail == cur)
        list->tail = cur->prev;

    if (cur->prev)
        cur->prev->next = cur->next;

    if (cur->next)
        cur->next->prev = cur->prev;

    Destroy_SP_ListNode(cur);
    return poped;
}

void * SP_List_index (SP_List *list, int pos) {
    assert(pos < list->size && pos >= 0);
    SP_ListNode *node = list_index(list, pos);
    return node->data;
}

void SP_List_concat (SP_List *list, SP_List *another) {
    while (another->size) {
        SP_List_append(list, SP_List_pop(another, 0));
    }
}

SP_List * SP_List_copy (SP_List *list) {
    SP_List *copy = Create_SP_List();
    SP_ListNode *cur = list->head;

    if (cur){
        do {
            SP_List_append(copy, cur->data);
            cur = cur->next;
        } while (cur);
    }
    return copy;
}

void SP_List_del (SP_List *list) {
    while (list->size) {
        (void) SP_List_pop(list, 0);
    }
}

void Destroy_SP_List (SP_List *list) {
    SP_List_del(list);
    SP_Free(list);
}
