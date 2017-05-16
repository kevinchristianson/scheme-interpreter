#include "interpreter.h"
#include "linkedlist.h"
#include "talloc.h"
#include <stdio.h>
#include "value.h"
#include <string.h>
#include "parser.h"

//helper function
//prints first item in value list
void printer(Value *expr){
    switch (expr->type) {
        case INT_TYPE:
            printf("%i\n", expr->i);
            break;
        case DOUBLE_TYPE:
            printf("%f\n", expr->d);
            break;
        case STR_TYPE:
            printf("%s\n", expr->s);
            break;
        case NULL_TYPE:
            break;
        case CONS_TYPE:
            printer(car(expr));
            break;
        case PTR_TYPE:
            break;
        case BOOL_TYPE:
            printf("%i\n", expr->i);
            break;
        case OPEN_TYPE:
            break;
        case CLOSE_TYPE:
            break;
        case SYMBOL_TYPE:
            printf("%s\n", expr->s);
            break;
    }
}



//creates an empty frame, calls eval on all tree branches
void interpret(Value *tree){
    Value *current = tree;
    Frame *frame = talloc(sizeof(Frame));
    frame->parent = 0;
    frame->bindings = makeNull();
    while(current->type != NULL_TYPE){
        printer(eval(car(current), frame));
        current = cdr(current);
    }
}


//helper function
//returns a value for a given variable
Value *lookUpSymbol(Value *expr, Frame *frame){
    Value *current = frame->bindings;
    if(frame->parent == 0){
        printf("Error: Symbol is not defined\n");
        texit(1);
    }
    while(current->type != NULL_TYPE){
        if (!strcmp(car(current)->s,expr->s)) {
            return car(cdr(current));
        }
        current = cdr(cdr(current));
    }
    return lookUpSymbol(expr, frame->parent);
}

// returns the number of tokens .in an expression
int checkParamNumber(Value *expr){
    Value * current = expr;
    int total_args = 0;
    while(current->type != NULL_TYPE){
        current = cdr(current);
        total_args += 1;
    }
    return total_args;
}

//evaluates if statements and returns the proper values
Value *evalIf(Value *args, Frame *frame){
    if(checkParamNumber(args) != 3){
        printf("ERROR in IF statement: expected 3 parameters, got %i\n", checkParamNumber(args));
        texit(1);
    }
    
    //if there are enough arguements, do if statement
    if(eval(car(args), frame)->i == 1){
        return eval(car(cdr(args)), frame);
    }
    return eval(car(cdr(cdr(args))), frame);
}

//evaluates let statements and returns the proper values
Value *evalLet(Value *args, Frame *frame){
    
    if(checkParamNumber(args) < 2){
        printf("ERROR in LET statement: expected 2 parameters, got %i\n", checkParamNumber(args));
        texit(1);
    }
    
    Frame *new = talloc(sizeof(Frame));
    new->bindings = makeNull();
    new->parent = frame;
    //assigns is the sub-tree containing all variable declarations
    Value *value_list = car(args);
    
    while(value_list -> type != NULL_TYPE){
        Value *pair = car(value_list);
        if(checkParamNumber(pair) != 2){
            printf("ERROR in LET assignment statement: expected 2 parameters, got %i\n", checkParamNumber(pair));
            texit(1);
        }
        new->bindings = cons(eval(car(cdr(pair)), frame), new->bindings);
        new->bindings = cons(car(pair), new->bindings);
        value_list = cdr(value_list);
    }
    
    while (cdr(args)->type !=NULL_TYPE){
        args = cdr(args);
    }
    
    return eval(car(args), new);

}


// Evaluates an expression and returns the proper values
Value *eval(Value *expr, Frame *frame){
    switch (expr->type)  {
        case INT_TYPE: {
            return expr;
            break;
        }
        case DOUBLE_TYPE: {
            return expr;
            break;
        }
        case STR_TYPE: {
            return expr;
            break;
        }
        case NULL_TYPE: {
            return makeNull();
            break;
        }
        case PTR_TYPE: {
            return makeNull();
            break;
        }
        case OPEN_TYPE: {
            return makeNull();
            break;
        }
        case CLOSE_TYPE: {
            return makeNull();
            break;
        }
        case SYMBOL_TYPE: {
            return lookUpSymbol(expr, frame);
            break;
        }
        case BOOL_TYPE: {
            return expr;
        }
        case CONS_TYPE: {
            
            Value *first = car(expr);
            Value *args = cdr(expr);
            Value *result;
            
            if(first->type != SYMBOL_TYPE){
                printf("ERROR: Attempting to evaluate a non-string as a special form!\n");
                texit(1);
            }
            
            if (!strcmp(first->s,"if")) {
                result = evalIf(args, frame);
                return result;
            }

            if (!strcmp(first->s,"let")){
                result = evalLet(args, frame);
                return result;
            }
            else {
                // not a recognized special form
                printf("ERROR: Not a recognized special form\n");
                texit(1);
            }
            return result;
            break;
        }
    }
}
