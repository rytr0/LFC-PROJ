#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "header.h"

extern symrec * table;
void yyerror(char * );

/***
 *     _____                 _   _             _          ______                    _  _
 *    |_   _|               | \ | |           | |         |  ___|                  (_)| |
 *      | | _ __  ___   ___ |  \| |  ___    __| |  ___    | |_  _   _  _ __    ___  _ | |_  ___   _ __   ___
 *      | || '__|/ _ \ / _ \| . ` | / _ \  / _` | / _ \   |  _|| | | || '_ \  / __|| || __|/ _ \ | '_ \ / __|
 *      | || |  |  __/|  __/| |\  || (_) || (_| ||  __/   | |  | |_| || | | || (__ | || |_| (_) || | | |\__ \
 *      \_/|_|   \___| \___|\_| \_/ \___/  \__,_| \___|   \_|   \__,_||_| |_| \___||_| \__|\___/ |_| |_||___/
 *
 *
 */
treeNode * init(){
  treeNode * res;
  if((res = malloc(sizeof(treeNode))) == NULL){
    yyerror("Can not allocate more memory");
    exit(OUT_OF_MEMORY);
  }
  return res;
}

/*
* costruisce un nodo constant dove si viene salvato il valore dell'espressione costante letta
*/
treeNode * constantNode(const basicType bt, ... ){
  treeNode * res = init();
  va_list ap;
  va_start(ap, bt);
  //init const
  constant c;
  c.type = bt;
  switch (c.type) {
    case basic_int_value:
      c.value.int_value = va_arg(ap,int);
      break;
    case basic_float_value:
      yyerror("not implemented yet");
      exit(NOT_IMPLEMENTED_YET);
      break;
    case basic_boolean_value:
      c.value.bool_value = (va_arg(ap,int) >= 1 ? true : false);
      break;
    case undef:
    default:
      yyerror("unmanaged type in costant node");
      exit(BUGGY_THE_CLOWN);
      break;
  }
  va_end(ap);
  res->type = const_type;
  res->value.con = c;
  return res;
}

treeNode * identifierNode(const char * varName){
  treeNode * res = init();
  identifier id;
  id.name = malloc(strlen(varName)+1);
  strcpy(id.name, varName);
  res->value.id =id;
  res->type = identifier_type;
  return res;
}

treeNode * opr(int oper, int nops, ...){
  va_list ap;
  treeNode *node = init();
  int i;
  if((node->value.expr.op = malloc(nops*sizeof(treeNode)))== NULL){
      yyerror("opr - No memory left for allocation");
      exit(OUT_OF_MEMORY);
  }
  node->type = expression_type;
  node->value.expr.operator = oper;
  node->value.expr.noperands = nops;
  va_start(ap, nops);
  for(i = 0; i<nops;i++){
      node->value.expr.op[i]=va_arg(ap,treeNode*);
  }
  va_end(ap);
  return node;
}

treeNode * fpCall(const char * name , actual * args){
  treeNode *node = init();
  routineNode rtn;
  rtn.name = malloc(strlen(name)+1);
  strcpy(rtn.name, name);
  rtn.args = args;
  node->type = routine_type;
  node->value.routine = rtn;
  return node;
}


/***
 *     _______   ____  _________  _____ _       _____ ___  ______ _      _____  ______ _   _ _   _ _____ _____ _____ _____ _   _  _____
 *    /  ___\ \ / /  \/  || ___ \|  _  | |     |_   _/ _ \ | ___ \ |    |  ___| |  ___| | | | \ | /  __ \_   _|_   _|  _  | \ | |/  ___|
 *    \ `--. \ V /| .  . || |_/ /| | | | |       | |/ /_\ \| |_/ / |    | |__   | |_  | | | |  \| | /  \/ | |   | | | | | |  \| |\ `--.
 *     `--. \ \ / | |\/| || ___ \| | | | |       | ||  _  || ___ \ |    |  __|  |  _| | | | | . ` | |     | |   | | | | | | . ` | `--. \
 *    /\__/ / | | | |  | || |_/ /\ \_/ / |____   | || | | || |_/ / |____| |___  | |   | |_| | |\  | \__/\ | |  _| |_\ \_/ / |\  |/\__/ /
 *    \____/  \_/ \_|  |_/\____/  \___/\_____/   \_/\_| |_/\____/\_____/\____/  \_|    \___/\_| \_/\____/ \_/  \___/ \___/\_| \_/\____/
 *
 */


symrec * createSym(char const * varName, symrec ** symbolTable){
     symrec * s;
     char * variableName = malloc(strlen(varName)+1);
     strcpy(variableName,varName);
     s = putSym(variableName, symbolTable);
     return s;
 }

 symrec * putSym(char const * identifier, symrec ** symbolTable){
   symrec *ptr = malloc (sizeof (symrec));
   ptr->name = (char *) malloc (strlen (identifier) + 1);
   strcpy (ptr->name,identifier);

   ptr->next = *symbolTable;
   *symbolTable = ptr;

   return ptr;
}

/**
cerca un simbolo nella tabella dei simboli indicata
*/
symrec * getSym(char const *identifier, symrec * symTable){
  symrec *ptr;
  for (ptr = symTable; ptr != (symrec *) 0; ptr = ptr->next){
    if (strcmp (ptr->name, identifier) == 0)
        return ptr;
    }

  return NULL;
}


treeNode * varDec(char * name, bool constant, type * dataType, ...){
  va_list ap;
  treeNode *node = init();
  node->type = identifier_declaration;
  node->value.dec.name = malloc(strlen(name)+1);
  strcpy(node->value.dec.name, name);
  node->value.dec.isCostant = constant;
  node->value.dec.t = dataType;
  if(dataType == NULL){
    yyerror("data type reached is null");
  }
  va_start(ap, dataType);
  node->value.dec.expr = va_arg(ap,treeNode*);
  va_end(ap);
  return node;
}





/***
 *      ___  ____________  _____   __ ______ _   _ _   _ _____ _____ _____ _____ _   _  _____
 *     / _ \ | ___ \ ___ \/ _ \ \ / / |  ___| | | | \ | /  __ \_   _|_   _|  _  | \ | |/  ___|
 *    / /_\ \| |_/ / |_/ / /_\ \ V /  | |_  | | | |  \| | /  \/ | |   | | | | | |  \| |\ `--.
 *    |  _  ||    /|    /|  _  |\ /   |  _| | | | | . ` | |     | |   | | | | | | . ` | `--. \
 *    | | | || |\ \| |\ \| | | || |   | |   | |_| | |\  | \__/\ | |  _| |_\ \_/ / |\  |/\__/ /
 *    \_| |_/\_| \_\_| \_\_| |_/\_/   \_|    \___/\_| \_/\____/ \_/  \___/ \___/\_| \_/\____/
 *
 *
 */

type * arrayDec(int size, type * t, basicType bt){
  type * res = malloc(sizeof(type));
  res->size = size;
  if(t==NULL){
    res->dt = basic_dataType;
    res->typeValue.bt = bt;
    res->t = NULL;
  }else{
    res->dt = complex_dataType;
    res->typeValue.ct = complex_array;
    res->t = t;
  }
  return res;
}

type * basicDec(basicType bt){
  type * res = malloc(sizeof(type));
  res->size = 0;
  res->dt = basic_dataType;
  res->typeValue.bt = bt;
  res->t = NULL;
  return res;
}

/***
 *    ______  ___  ______  ___  ___  ___ _____ _____ ___________  _____
 *    | ___ \/ _ \ | ___ \/ _ \ |  \/  ||  ___|_   _|  ___| ___ \/  ___|
 *    | |_/ / /_\ \| |_/ / /_\ \| .  . || |__   | | | |__ | |_/ /\ `--.
 *    |  __/|  _  ||    /|  _  || |\/| ||  __|  | | |  __||    /  `--. \
 *    | |   | | | || |\ \| | | || |  | || |___  | | | |___| |\ \ /\__/ /
 *    \_|   \_| |_/\_| \_\_| |_/\_|  |_/\____/  \_/ \____/\_| \_|\____/
 *
 *
 */

//TODO: check if is possible to optimize this code
form * formList(form * new, form ** list){
    form ** tmp = &((*list)->next);
    while(*tmp!=NULL){
      tmp = &((*tmp)->next);
    }
    *tmp = new;
    return (*list);
}

//implemented considering only basic types
form * newParam(const char * paramName, dataType dt, ...){
  form * res = malloc(sizeof(form));
  res->name = malloc(strlen(paramName)+1);
  strcpy(res->name, paramName);
  res->type = dt;
  va_list ap;
  va_start(ap, dt);

  switch (dt) {
    case basic_dataType:
      (*res).bt =  va_arg(ap,basicType);
      break;
    case complex_dataType:
      (*res).ct = va_arg(ap,complexType);;
      break;
    default:
      yyerror("unmanaged type in parameter list.");
      exit(BUGGY_THE_CLOWN);
      break;
  }
  va_end(ap);
  res->next = NULL;
  return res;
}

// void printFormList(form * lista){
//   form * f;
//   int i = 0;
//   for(f=lista;f!=NULL; f=f->next){
//     write_log(NULL,logInt("param %d",i++));
//     write_log(NULL,logString("param name %s", f->name));
//   }
// }

 //next param is the return type, just basic yet
 //implemented thinking only about basic types
routine * newRoutine(const char * name,form * formals, treeNode * statements, ...){
  routine * res = malloc(sizeof(routine));
  res->name = malloc(strlen(name)+1);
  strcpy(res->name,name);
  res->parameters = formals;
  res->statementList = statements;
  va_list ap;
  va_start(ap, statements);
  type * t = va_arg(ap,type*);
  //switch (t->dataType) etc etc

  if(t == NULL || t->typeValue.bt == undef){
    res->bt = undef;
    res->type = procedure;
  }else{
    res->type = function;
    res->bt = t->typeValue.bt;

  }
  va_end(ap);
  res->returnType = basic_dataType;
  return res;
}

list * addToList(routine * newRoutine, list ** rList){
  list * l = malloc(sizeof(list));
  l->r = newRoutine;
  l->type = routine_list;
  if(rList == NULL){
    return l;
  }
  l->next = *rList;
  *rList = l;
  return l;
}

// void printAllProcAndFun(list * rList){
//   if(rList == NULL){
//     write_log(NULL,"empty list");
//   }
//   list * l;
//   for(l = rList; l!=NULL; l = l->next){
//     write_log(NULL,logString("name %s", l->r->name));
//   }
// }

actual * newActual(treeNode * expr){
  actual * act = malloc(sizeof(actual));
  act->expr = expr;
  act->next = NULL;
  return act;
}

actual * addToActList(actual * new, actual ** list){
  actual ** tmp = &((*list)->next);
  while(*tmp!=NULL){
    tmp = &((*tmp)->next);
  }
  *tmp = new;
  return (*list);
}
int actLength(actual * args){
  int count = 0;
  if(args==NULL){
    return count;
  }
  actual * temp;
  for(temp = args; temp!=NULL; temp = temp->next){
    count++;
  }
  return count;
}
int formLength(form * forms){
  int count = 0;
  if(forms==NULL){
    return count;
  }
  form * temp;
  for(temp = forms; temp!=NULL; temp = temp->next){
    count++;
  }
  return count;
}

routine * getRoutine(const char * name, list * routineList){
  if(routineList == NULL){
    return NULL;
  }
  //TODO typecheking about routineList being really a list of type routine
  routine *ptr;
  list * tmp;
  //write_log(NULL,logString("looking for %s",name));
  for (tmp = routineList; tmp!=NULL; tmp=tmp->next){
    ptr = tmp->r;
    //write_log(NULL,logString("name found %s",ptr->name));
    if(strcmp(ptr->name, name)==0){
      return ptr;
    }
  }
  return NULL;

}

form * getFormAtIndex(int index, form * list){
  form * tmp;
  int i = 0;
  for(tmp = list; tmp!=NULL; tmp=tmp->next){
    if(i == index){
      return tmp;
    }
    i++;
  }
  return NULL;
}

actual * getActualAtIndex(int index, actual * list){
  actual * tmp;
  int i = 0;
  for(tmp = list; tmp!=NULL; tmp=tmp->next){
    if(i == index){
      return tmp;
    }
    i++;
  }
  return NULL;
}

//wrapper for getSym
symrec * getSymbolFromIdentifier(identifier identifierNode, symrec ** symbolTable){
 symrec * s = getSym(identifierNode.name, *symbolTable);
 if(s == 0){
   //TODO: place variable name in output error
   //yyerror("VARIABLE NOT FOUND");
   //exit(NO_SUCH_VARIABLE);
 }
 return s;
}

//makes a pointer out of the data type
//simplied cause we do not manage complex return type, from functions...
data * dataToDataPointer(data d){
  data * res = malloc(sizeof(data));
  //TODO: check if we have a pointer or null
  res->type = d.type;
  switch (d.type) {
    case basic_dataType:
    {
      basic bRes;
      switch (d.b.type) {
        case basic_int_value:
          bRes.type = basic_int_value;
          bRes.i = d.b.i;
        break;
        case basic_float_value:
          yyerror("BOOOOM you get so far, how? float is not implemented");
          exit(NOT_IMPLEMENTED_YET);
        break;
        case basic_boolean_value:
          bRes.type = basic_boolean_value;
          bRes.b = d.b.b;
        break;
        default:
          yyerror("new data type not implemented in dataToDataPointer");
          exit(NOT_IMPLEMENTED_YET);
      }
      res->b = bRes;
    }
    break;
    case complex_dataType:
      yyerror("complex_dataType not implemented in dataToDataPointer");
    default:
      yyerror("unmanaged data copy");
      exit(BUGGY_THE_CLOWN);
  }
  return res;
}


 /***
  *       _  _    _____  _   _ ______
  *     _| || |_ |  ___|| \ | ||  _  \
  *    |_  __  _|| |__  |  \| || | | |
  *     _| || |_ |  __| | . ` || | | |
  *    |_  __  _|| |___ | |\  || |/ /
  *      |_||_|  \____/ \_| \_/|___/
  *
  *
  */
