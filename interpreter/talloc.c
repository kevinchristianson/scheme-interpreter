// Written by Jack Kennelly, Kevin Christianson, and Holden Greenberg.
// Implementation of a global linked list to track pointers for easy garbage collection.

#include <stdlib.h>
#include "value.h"
#include <stdio.h>
#include "talloc.h"


Value *list; // Global variable used to store points for easy garbage collection.
int flag = -1; // Flag to check whether list has been instantiated or not.

// Create a new CONS_TYPE value node. Works functionally the same as the version in
// linkedlist, but it has been renamed to avoid linking errors.
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
    if (flag == -1){
        list = (Value *) malloc(sizeof(Value));
        list -> type = NULL_TYPE;
        flag = 1;
    }
    Value *new = (Value *) malloc(size);
    new -> type = CONS_TYPE;
    list = innerCons(new, list);
    return new;
    
}

// Frees up all memory directly or indirectly referred to by list.
// Used as a helper function for tfree.
void cleanup(Value *list){
  while(list->type != NULL_TYPE){
    Value *next = list->c.cdr;
    switch (list->c.car->type) {
      case INT_TYPE:
        free(list->c.car);
        break;
      case DOUBLE_TYPE:
        free(list->c.car);
        break;
      case STR_TYPE:
        free(list->c.car);
        break;
      case NULL_TYPE:
        free(list->c.car);
        break;
      case CONS_TYPE:
        free(list->c.car);
        break;
      case PTR_TYPE:
        free(list->c.car);
        break;
    }
    free(list);
    list = next;
  }
  free(list);
}

// Free all pointers allocated by talloc, as well as whatever memory you
// allocated in lists to hold those pointers.
void tfree(){
    cleanup(list);
    flag = -1;
}

// Replacement for the C function "exit", that consists of two lines: it calls
// tfree before calling exit. It's useful to have later on; if an error happens,
// you can exit your program, and all memory is automatically cleaned up.
void texit(int status){
    tfree();
    exit(0);
}
