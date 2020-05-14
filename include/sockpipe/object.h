#ifndef SOCKPIPE_OBJECT
#define SOCKPIPE_OBJECT

#include "sockpipe/magic.h"
#include "sockpipe/alloc.h"
#include <string.h>

/* Define type SP_Object */

#define TYPE_SP_Object 0

DEF_META(SP_Object) {
    type_t type; 
    
    /* Add virtual functions and static class variables here */
};

typedef struct _SP_Object {
    META(SP_Object);

} SP_Object;

void SP_Object_init(SP_Object *self);
void SP_Object_del(SP_Object *self);

GEN_Create(SP_Object)
GEN_Destroy(SP_Object)

/* For virtual functions, define a inline method that invoke the virtual one */

/*
 * GEN_Virtual(SP_Object, del);
 */

#endif
