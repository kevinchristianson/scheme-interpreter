#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "linkedlist.h"

// Create a new NULL_TYPE value node.
Value *makeNull(){
  Value *lst;
  lst = (Value *) malloc(sizeof(Value));
  lst->type = NULL_TYPE;
  return lst;
}

// Create a new CONS_TYPE value node.
Value *cons(Value *car, Value *cdr){
  Value *lst;
  lst = (Value *) malloc(sizeof(Value));
  lst->type = CONS_TYPE;
  lst->c.car = car;
  lst->c.cdr = cdr;
  return lst;
}

// Display the contents of the linked list to the screen in some kind of
// readable format
void display(Value *list){
  assert(list->type == CONS_TYPE);
  printf("(");
  while(list->type != NULL_TYPE){
    switch (list->c.car->type) {
      case INT_TYPE:
        printf("%i ", list->c.car->i);
        break;
      case DOUBLE_TYPE:
        printf("%f ", list->c.car->d);
        break;
      case STR_TYPE:
        printf("%s ", list->c.car->s);
        break;
      case NULL_TYPE:
        break;
      case CONS_TYPE:
        break;
    }
    list = list -> c.cdr;
  }
  printf(")\n");
}

// Return a new list that is the reverse of the one that is passed in. All
// content within the list should be duplicated; there should be no shared
// memory between the original list and the new one.
//
// FAQ: What if there are nested lists inside that list?
// ANS: There won't be for this assignment. There will be later, but that will
// be after we've got an easier way of managing memory.
Value *reverse(Value *list){
  Value *new = makeNull();
  while(list->type != NULL_TYPE){
    new = cons(list->c.car,new);
    list = list -> c.cdr;
  }
  return new;
}

// Frees up all memory directly or indirectly referred to by list. Note that
// this linked list might consist of linked lists as items, so you'll need to
// clean them up as well.
// FAQ: What if there are nested lists inside that list?
// ANS: There won't be for this assignment. There will be later, but that will
// be after we've got an easier way of managing memory.
void cleanup(Value *list){
  while(list->type != NULL_TYPE){
    Value *next = list->c.cdr;
    //free(list->c.car->type);
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
    }
    free(list);
    list = next;
  }
  free(list);
}

// Utility to make it less typing to get car value. Use assertions to make sure
// that this is a legitimate operation.
Value *car(Value *list){
  assert(list->type == CONS_TYPE);
  return list->c.car;
}

// Utility to make it less typing to get cdr value. Use assertions to make sure
// that this is a legitimate operation.
Value *cdr(Value *list){
  assert(list->type == CONS_TYPE);
  return list->c.cdr;
}

// Utility to check if pointing to a NULL_TYPE value. Use assertions to make sure
// that this is a legitimate operation.
bool isNull(Value *value){
  assert(value->type == NULL_TYPE || value->type == CONS_TYPE);
  return (value->type == NULL_TYPE);
}

// Measure length of list. Use assertions to make sure that this is a legitimate
// operation.
int length(Value *value){
  assert(value->type == CONS_TYPE);
  int counter = 0;
  while(value->type != NULL_TYPE){
    counter++;
    value = value->c.cdr;
  }
  return counter;
}
