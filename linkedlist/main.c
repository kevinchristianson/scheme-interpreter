// Tester for linked list. Note that this does not test all the functionality
// you implemented; you should enhance it significantly. We will test your
// linked list code with a different main.c.

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "linkedlist.h"
#include "value.h"

int main() {
    Value *val1 = malloc(sizeof(Value));
    val1->type = INT_TYPE;
    val1->i = 12;

    Value *val2 = malloc(sizeof(Value));
    val2->type = DOUBLE_TYPE;
    val2->d = 4.3;

    Value *val3 = malloc(sizeof(Value));
    val3->type = STR_TYPE;
    val3->s = "hello";

    Value *head = makeNull();
    head = cons(val1,head);
    head = cons(val2,head);
    head = cons(val3, head);

    Value *rev = reverse(head);
    
    Value *nullCheck = makeNull();

    display(head);
    display(rev);
    printf("Length = 4? %i\n",length(head));
    printf("Empty = 0? %i\n",isNull(head));
    printf("Empty = 1? %i\n", isNull(nullCheck));
    //free(val1);
    //free(val2);
    //free(val3);
    cleanup(nullCheck);
    cleanup(head);
    cleanup(rev);
}
