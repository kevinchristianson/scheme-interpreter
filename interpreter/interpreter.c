#include "interpreter.h"
#include "linkedlist.h"
#include "talloc.h"

void interpret(Value *tree){
    Value *current = tree;
    while(current->type != NULL_TYPE){
        eval(car(current));
        current = cdr(current);
    }
}

Value *lookUpSymbol(Value *expr, Frame *frame){
    if(frame == NULL_TYPE){
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

// returns the number of tokens in an expression
int checkParamNumber(Value *expr){
    Value * current = expr;
    int total_args = 0;
    while(current->type != NULL_TYPE){
        current = cdr(current);
        total_args += 1;
    }
    return total_args;
}

Value *evalIf(Value *args, Frame *frame){
    if(checkParamNumber(args) < 3){
        printf("ERROR in IF statement: expected 3 parameters, got %i", checkParamNumber(args));
        texit(1);
    }
    //if there are enough arguements, do if statement
    if(eval(args)){
        return cdr(args);
    }
    return cdr(cdr(args));
}

Value *evalLet(Value *args, Frame *frame){
    if(checkParamNumber(args) < 2){
        printf("ERROR in LET statement: expected 2 parameters, got %i", checkParamNumber(args));
        texit(1);
    }
    Frame *new = talloc(sizeof(Frame));
    new->bindings = makeNull();
    new->parent = frame;
    //assigns is the sub-tree containing all variable declarations
    assigns = car(args)
    while(assigns->type != NULL_TYPE){
        if(checkParamNumber(car(assigns) != 2){
            printf("ERROR in LET assignment statement: expected 2 parameters, got %i", checkParamNumber(args));
            texit(1);
        }
        cons(car(assigns)->s, new);
        cons(eval(cdr(car(assigns)), frame), new);
        assigns = cdr(assigns);
    }
    return eval(cdr(args), new);

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
                result = evalIf(args, frame);
            }

            if (!strcmp(first->s,"let")){
                result = evalLet(args, frame);
            }
            else {
                // not a recognized special form
                evalationError();
            }
            break;
        }
    }
}
