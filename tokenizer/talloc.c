#include <stdlib.h>
#include "value.h"
#include <stdio.h>
#include "talloc.h"

Value *HEAD;
int FLAG = 0;

// Create a new CONS_TYPE value node.
Value *innerCons(Value *car, Value *cdr){
  Value *lst = (Value *) malloc(sizeof(Value));
  lst->type = CONS_TYPE;
  lst->c.car = car;
  lst->c.cdr = cdr;
  return lst;
}

// Replacement for malloc that stores the pointers allocated. It should store
// the pointers in some kind of list; a linked list would do fine, but insert
// here whatever code you'll need to do so; don't call functions in the
// pre-existing linkedlist.h. Otherwise you'll end up with circular
// dependencies, since you're going to modify the linked list to use talloc.
void *talloc(size_t size){
    if(FLAG == 0){
      HEAD = (Value *) malloc(sizeof(Value));
      HEAD->type = NULL_TYPE;
      FLAG = 1;
    }
    Value *new = (Value *) malloc(size);
    new->type = CONS_TYPE;
    HEAD = innerCons(new, HEAD);
    return new;
}

void cleanup(Value *list){
    if(list->type == CONS_TYPE){
      cleanup(list->c.car);
      cleanup(list->c.cdr);
    }
    free(list);
}

// Free all pointers allocated by talloc, as well as whatever memory you
// allocated in lists to hold those pointers.
void tfree(){
    cleanup(HEAD);
    FLAG = 0;
    HEAD = 0;
}

// Replacement for the C function "exit", that consists of two lines: it calls
// tfree before calling exit. It's useful to have later on; if an error happens,
// you can exit your program, and all memory is automatically cleaned up.
void texit(int status){
    tfree();
    exit(0);
}
