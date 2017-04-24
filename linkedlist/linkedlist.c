#import <stdio.h>


struct Value {
    valueType type; // type will also have a CONS_TYPE as an option
    union {
        int i;
        double d;
        float f;
        char c;
        struct ConsCell {
            struct Value *car;
            struct Value *cdr;
        } c;
    };
};


typedef struct Value Value;
