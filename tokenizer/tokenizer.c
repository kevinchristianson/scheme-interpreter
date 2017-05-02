#include "value.h"
#include "tokenizer.h"
#include <string.h>
#include "linkedlist.h"
#include "talloc.h"
#include <stdio.h>
#include <assert.h>

// Read all of the input from stdin, and return a linked list consisting of the
// tokens.
Value *tokenize(){
    char charRead;
    Value *list = makeNull();
    charRead = fgetc(stdin);
    char* symbolSet = "!$%&*/:<=>?~_^";
    char* letterSet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char* numberSet = "0123456789";
    while (charRead != EOF) {
        if (charRead == '(') {
            Value *val = talloc(sizeof(Value));
            val->type = OPEN_TYPE;
            val->s = "(";
            cons(val, list);
        } else if (charRead == ')') {
            Value *val = talloc(sizeof(Value));
            val->type = CLOSE_TYPE;
            val->s = ")";
            cons(val, list);
        } else if (charRead == ';'){
            while (charRead != '\n'){
                charRead = fgetc(stdin);
            }
        } else if (strchr(symbolSet, charRead)){
            Value *val = talloc(sizeof(Value));
            val->type = SYMBOL_TYPE;
            if(charRead == '#'){
                val->s = talloc(3*sizeof(char));
                val->s[0] = charRead;
                charRead = fgetc(stdin);
                if(charRead == 't' || charRead == 'f'){
                    val->s[1] = charRead;
                }else{
                    printf("%s\n", "Syntax error: untokenizable");
                }
            }else{
                val->s = talloc(2*(sizeof(char)));
                val->s[0] = charRead;
            }
            cons(val, list);
        } else if (strchr(letterSet, charRead)){
            char *temp = talloc(100*sizeof(char));
            int counter = 0;
            while (charRead != ' '){
                temp[counter] = charRead;
                counter++;
                charRead = fgetc(stdin);
            }
            Value *val = talloc(sizeof(Value));
            val->type = STR_TYPE;
            val->s = temp;
            cons(val, list);
        } else if(strchr(numberSet, charRead)){
            char *temp = talloc(100*sizeof(char));
            int counter = 0;
            int flag = 0;
            while (charRead != ' '){
                temp[counter] = charRead;
                counter++;
                charRead = fgetc(stdin);
                if (charRead == '.'){
                    flag = 1;
                }
            }
            //Still need to convert string to integer / double
            Value *val = talloc(sizeof(Value));
            if (flag == 0){
                val->type = INT_TYPE;
                sscanf(temp, "%i", &val->i);
            } else {
                val->type = (double) DOUBLE_TYPE;
                sscanf(temp, "%d", &val->i);
            }
            cons(val, list);
        }
        charRead = fgetc(stdin);
    }
    Value *revList = reverse(list);
    return revList;
}

/* Value *makeValue(valueType type, void *value){
    Value *val = talloc(sizeof(Value));
    val->type = type;
    switch (type) {
    case INT_TYPE:
      val->i = value;
      break;
    case DOUBLE_TYPE:
      val->d = value;
      break;
    case STR_TYPE:
      val->s = value;
      break;
    case NULL_TYPE:
      break;
    case CONS_TYPE:
      break;
    case PTR_TYPE:
      break;
    case OPEN_TYPE:
      val->s = value;
      break;
    case CLOSE_TYPE:
      val->s = value;
      break;
    case BOOL_TYPE:
      val->i = value;
      break;
    case SYMBOL_TYPE:
      val->s = value;
      break;
  }
   return val;
} */

// Displays the contents of the linked list as tokens, with type information
void displayTokens(Value *list){
    assert(list->type == CONS_TYPE);
    while(list->type != CONS_TYPE){
        switch (list->c.car->type) {
            case INT_TYPE:
                printf("%i%s", list->c.car->i, " :integer");
                break;
            case DOUBLE_TYPE:
                printf("%f%s", list->c.car->d, " :float");
                break;
            case STR_TYPE:
                printf("%s%s", list->c.car->s, " :string");
                break;
            case NULL_TYPE:
                break;
            case CONS_TYPE:
                break;
            case PTR_TYPE:
                break;
            case OPEN_TYPE:
                printf("%s%s", list->c.car->s, " :open");
            case CLOSE_TYPE:
                printf("%s%s", list->c.car->s, " :close");
            case BOOL_TYPE:
                printf("%i%s", list->c.car->i, " :boolean");
            case SYMBOL_TYPE:
                printf("%s%s", list->c.car->s, " :symbol");
        }
        list = list -> c.cdr;
        printf(")\n");
  }
  printf(")\n");
}
