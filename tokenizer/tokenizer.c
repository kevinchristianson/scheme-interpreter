#include "value.h"
#include "tokenizer.h"
#include <string.h>

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
            char *temp = talloc(sizeof(char));
            temp[0] = charRead;
            val->s = temp;
            cons(val, list);
        } else if (strchr(letterSet, charRead)){
            char *temp = talloc(sizeof(char));
            while (charRead != ' '){
                
                charRead = fgetc(stdin);
            }
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

}
