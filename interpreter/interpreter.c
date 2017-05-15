#include "interpreter.h"


void interpret(Value *tree){
    Value *current = tree;
    while(current->type != NULL_TYPE){
        eval(car(current));
        current = cdr(current);
    }
}

Value *lookUpSymbol(Value *expr, Frame *frame){
    if(frame == NULL){
        printf("Error: %s is not defined\n", car(expr)->s);
        texit(1);
    }
    while(frame->bindings->type != NULL_TYPE){
        if (frame->car(bindings) == car(expr)){
            return car(cdr(bindings));
        }
    }
    return lookUpSymbol(expr, frame->parent);
}

Value *eval(Value *expr, Frame *frame){
    switch (expr->type)  {
        case INT_TYPE: {
            return expr->i;
            break;
        }
        case DOUBLE_TYPE: {
            return expr->d;
            break;
        }    
        case STR_TYPE: {
            return expr->s;
            break;
        }
        case NULL_TYPE: {
            return;
            break;
        }
        case PTR_TYPE: {
            return;
            break;
        }
        case OPEN_TYPE: {
            return;
            break;
        }
        case CLOSE_TYPE: {
            return;
            break;
        }
        case SYMBOL_TYPE: {
            return lookUpSymbol(expr, frame);
            break;
        }
        case BOOL_TYPE: {
            return expr->i;
        }
        case CONS_TYPE: {
            Value *first = car(expr);
            Value *args = cdr(expr);
            // Sanity and error checking on first...
            
            if(first->type != SYMBOL_TYPE){
                printf("ERROR: Attempting to evaluate a non-string as a special form!/n");
                texit(1);
            }
            if (!strcmp(first->s,"if")) {
                result = evalIf(args,frame);
            }
            
            if (!strcmp(first->s,"let")){
                result = evalLet(args,frame);
            }
            else {
                // not a recognized special form
                evalationError();
            }
            break;
        }
    }    
}