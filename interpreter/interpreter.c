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
            printf("(");
            printTree(expr);
            printf(")\n");
            break;
        case PTR_TYPE:
            break;
        case BOOL_TYPE:
            if(expr->i){
                printf("#t\n");
            } else {
                printf("#f\n");
            }
            break;
        case OPEN_TYPE:
            break;
        case CLOSE_TYPE:
            break;
        case SYMBOL_TYPE:
            printf("%s\n", expr->s);
            break;
        case VOID_TYPE:
            break;
        case CLOSURE_TYPE:
            printf("ERROR: Should not have printed a closure type.\n");
            printTree(expr);
            break;
    }
}


Value *apply(Value *function, Value *args){
    Frame *newFrame = talloc(sizeof(Frame));
    newFrame->bindings = makeNull();
    newFrame->parent = function->cl.frame;
    Value *temp = function->cl.paramNames;
    while(args->type != NULL_TYPE && temp->type != NULL_TYPE){
        //printf("1\n");fflush(stdout);
        newFrame->bindings = cons(car(args), newFrame->bindings);
        //printf("2\n");fflush(stdout);
        newFrame->bindings = cons(car(temp), newFrame->bindings);
        //printf("3\n");fflush(stdout);
        temp = cdr(temp);
        args = cdr(args);
    }
    if(args->type != NULL_TYPE || temp->type != NULL_TYPE){
        printf("ERROR: Function parameters and arguments not of equal length\n");
        texit(1);
    }
    return eval(car(function->cl.functionCode), newFrame);
}

Value *evalEach(Value *args, Frame *frame){
    Value *temp = makeNull();
    while(args->type != NULL_TYPE){
        temp = cons(eval(car(args), frame), temp);
        args = cdr(args);
    }
    
    return reverse(temp);
}

//creates an empty frame, calls eval on all tree branches
void interpret(Value *tree){
    Value *current = tree;
    Frame *topFrame = talloc(sizeof(Frame));
    topFrame->parent = 0;
    topFrame->bindings = makeNull();
    while(current->type != NULL_TYPE){
        printer(eval(car(current), topFrame));
        current = cdr(current);
    }
}


//helper function
//returns a value for a given variable
Value *lookUpSymbol(Value *expr, Frame *frame){
    Value *current = frame->bindings;
    while(current->type != NULL_TYPE){
        if (!strcmp(car(current)->s,expr->s)) {
            return car(cdr(current));
        }
        current = cdr(cdr(current));
    }
    if(frame->parent == 0){
        printf("Error: is not defined\n");
        texit(1);
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
    if(eval(car(args), frame)->type != BOOL_TYPE){
        printf("ERROR: Expected boolean, received something else\n");
        texit(1);
    }
    if(eval(car(args), frame)->i == 1){
        return eval(car(cdr(args)), frame);
    }
    return eval(car(cdr(cdr(args))), frame);
}

Value *evalQuote(Value *args, Frame *frame){
    return args;
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

Value *evalDefine(Value *expr, Frame *frame){
    Frame *tempFrame = frame;
    if(checkParamNumber(expr) != 2){
        printf("ERROR in define: expected 2 arguments\n");
        texit(1);
    }
    while(tempFrame->parent != 0){
        tempFrame = tempFrame->parent;
    }
    tempFrame->bindings = cons(eval(car(cdr(expr)), frame), tempFrame->bindings);
    tempFrame->bindings = cons(car(expr), tempFrame->bindings);
    
    Value *returnVal = makeNull();
    returnVal->type = VOID_TYPE;
    return returnVal;
}

Value *evalLambda(Value *expr, Frame *frame){
    Value *closure = talloc(sizeof(Value));
    closure->type = CLOSURE_TYPE;
    closure->cl.frame = frame;
    closure->cl.paramNames = car(expr);
    //printf("5\n");fflush(stdout);
    closure->cl.functionCode = cdr(expr);
    return closure;
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
        case VOID_TYPE: {
            return makeNull();
            break;
        }
        case CLOSURE_TYPE: {
            return makeNull();
            break;
        }
        case CONS_TYPE: {
            
            Value *first = car(expr);
            Value *args = cdr(expr);
            Value *result;
            
            if (!strcmp(first->s,"if")) {
                result = evalIf(args, frame);
                return result;
            }

            if (!strcmp(first->s,"let")){
                result = evalLet(args, frame);
                return result;
            }
            
            if (!strcmp(first->s,"quote")){
                result = evalQuote(args, frame);
                return result;
            }
            
            if (!strcmp(first->s,"define")){
                result = evalDefine(args, frame);
                return result;
            }
            if (!strcmp(first->s,"lambda")){
                result = evalLambda(args, frame);
                return result;
            }
            
            if(first->type != SYMBOL_TYPE && first->type != CONS_TYPE){
                printf("ERROR: Attempting to evaluate a non-string as a special form!\n");
                texit(1);
            }

            else {
                // If not a special form, evaluate the first, evaluate the args, then
                // apply the first to the args.
                Value *evaledOperator = eval(first, frame);
                Value *evaledArgs = evalEach(args, frame);
                return apply(evaledOperator,evaledArgs); 
            }
            return result;
            break;
        }
    }
}


