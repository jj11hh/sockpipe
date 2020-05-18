#ifndef SOCKPIPE_LIST
#define SOCKPIPE_LIST

#include "sockpipe/alloc.h"
#include "sockpipe/object.h"

struct _SP_ListNode;

typedef struct _SP_ListNode {
    void * data;
    struct _SP_ListNode* prev;
    struct _SP_ListNode* next;
} SP_ListNode;

typedef struct _SP_List {
    int size;
    struct _SP_ListNode* head;
    struct _SP_ListNode* tail;
} SP_List;

SP_ListNode * Create_SP_ListNode (void);
void SP_ListNode_init (SP_ListNode *);
void SP_ListNode_del (SP_ListNode *);
void Destroy_SP_ListNode (SP_ListNode *);
GEN_Cleanup(SP_ListNode)

SP_List * Create_SP_List (void);
void SP_List_init (SP_List *list);
void SP_List_insert (SP_List *list, int pos, void *data);
void SP_List_append (SP_List *list, void *data);
void SP_List_insert_before (SP_List *list, SP_ListNode *next, void *data);
void SP_List_append_after (SP_List *list, SP_ListNode *prev, void *data);
void * SP_List_replace (SP_List *list, int pos, void * replacement);
void * SP_List_pop (SP_List *list, int pos);
void * SP_List_index (SP_List *list, int pos);
void SP_List_concat (SP_List *list, SP_List *another);
SP_List * SP_List_copy(SP_List *list); //Shallow Copy
void SP_List_del (SP_List *list);
void Destroy_SP_List (SP_List *list);
GEN_Cleanup(SP_List)

#endif //SOCKPIPE_LIST
