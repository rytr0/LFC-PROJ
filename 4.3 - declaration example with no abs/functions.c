#include <stdio.h> /* for stderr */
#include <stdlib.h> /* malloc. */
#include <string.h> /* strlen. */
#include <syslog.h>
#include "header.h"

int indent = 0;
/**
prende un tipo e restituisce una stringa che lo descrive
*/
char * getDataTypeName (const enum data_type dt){
  char * tipo;
  switch (dt) {
      case dt_integer:
        tipo = "intero";
        break;
      case dt_float:
        tipo = "float";
        break;
      case dt_array:
        tipo = "array";
        break;
      case dt_structure:
        tipo = "struttura";
        break;
      default:
        printf("type not correctly managed");
        exit(-2);
  }
  return tipo;
}


/**
  crea un simbolo e lo mette nella mappa dei simboli
  è necessario
  <ul>
  <il>il nome del simbolo</li>
  <il>il type del simbolo</li>
  </ul>
*/
symrec * createSym(char const * varName, type * type){
  symrec * s;
  char * variableName = malloc(strlen(varName)+1);
  strcpy(variableName,varName);
  s = getsym(variableName);
  if (s == 0){
    s = putsym(variableName, type);
  }
  return s;
}
/**
  crea un simbolo e lo mette nella mappa dei simboli
  è necessario
  <ul>
  <il>il nome del simbolo</li>
  <il>il type del simbolo</li>
  <il>la tabella **</li> NOTA: vogliamo modificare un puntatore.. quindi ci serve un puntatore a puntatore..

  </ul>\
*/
symrec * createSymStruct(char const * varName, type * type, symrec ** tabella){
    symrec * s;
    char * variableName = malloc(strlen(varName)+1);
    strcpy(variableName,varName);
    s = getsymStruct(variableName, *tabella);
    if (s == 0){
        s = putsymStruct(variableName, type, tabella);
    }
    return s;
}

/**
inserisce un simbolo nella tabella dei simboli,
necessita di identificatore e tipo
*/
symrec * putsym (char const * identifier, type * tipo){
  symrec *ptr = (symrec *) malloc (sizeof (symrec));
  ptr->name = (char *) malloc (strlen (identifier) + 1);
  strcpy (ptr->name,identifier);
  ptr->tipo = tipo;
  ptr->next = (struct symrec *)sym_table;
  sym_table = ptr;
  return ptr;
}
/**
inserisce un simbolo nella tabella dei simboli,
necessita di identificatore e tipo e della tabella nella quale si vuole inserire il symbolo
*/
symrec * putsymStruct(char const * identifier, type * tipo, symrec ** tabella){
  symrec *ptr = (symrec *) malloc (sizeof (symrec));
  ptr->name = (char *) malloc (strlen (identifier) + 1);
  strcpy (ptr->name,identifier);
  ptr->tipo = tipo;
  ptr->next = *tabella;
  *tabella = ptr;
  return ptr;
}
/**
cerca un simbolo nella tabella dei simboli globale
*/
symrec * getsym (char const * identifier)
{
  symrec *ptr;
  for (ptr = sym_table; ptr != (symrec *) 0; ptr = (symrec *)ptr->next){
		if (strcmp (ptr->name, identifier) == 0)
      	return ptr;
  }
  return NULL;
}
/**
cerca un simbolo nella tabella dei simboli indicata
*/
symrec * getsymStruct(char const *identifier, symrec * tabella){
  symrec *ptr;
  for (ptr = tabella; ptr != (symrec *) 0; ptr = ptr->next){
    if (strcmp (ptr->name, identifier) == 0)
        return ptr;
    }
  return NULL;
}



int checkArrayAcces(array * a, ref * r){

    //controllo di non essere arrivato alla fine dell'array
    if(a->datatype == dt_array){
      if(r == NULL){
        printf("%s\n", "please access a specific element of the array");
        exit(-1);
      }
      if(r->n > a->size-1){
        printf("%s %d %s %d\n", "Index out of bound exception. Accessed item is", r->n, ", array length is",a->size);
        exit(-2);
      }else{
        return checkArrayAcces((a->value.a[r->n]), r->next);
      }
    }
    else{
      if(r == NULL){
        printf("%s\n", "stato inconsistente, verificare la logica");
        exit(-2);
      }
      //tipo di base.. avrò un array che contiene un riferimento a basic**
      if(r->n > a->size-1){
        printf("%s %d %s %d\n", "Index out of bound exception. Accessed item is", r->n, ", array length is",a->size);
        exit(-2);
      }
      else{
        //controllo res->next
        if(r->next!=NULL){
          printf("%s\n", "Index out of bound exception. Accessing an array which is not available");
          return 0;
        }
        else{
          return a->size;
        }
      }
    }
}
/**
  prendo il putatore a quell'elemento.. dovrei cambiare il tipo di ritorno
*/
int* arrayElement(array * a, ref * r){
  //il controllo è gia avvenuto a monte, vado diretto a prendere il puntatore

  if(a->datatype == dt_array){
    int accessedItem = r->n;
    //controllo che il prossimo elemento non sia la fine dell'array
    //pointer arthmetic - a->value.a = valore di base + offset (l'elemento che vogliamo prendere)
    return arrayElement((a->value.a[accessedItem]),r->next);
  }else{
    //sono nell'array fatto di tipi di base
    return &(a->value.b[r->n]->value.i);
  }

}


int typeChecking(symrec * variable){

  if(variable->tipo->datatype != dt_array ){
    if(variable->ref!= NULL){
        printf("%s %s\n %s %s\n %s\n", "accessing variablie",variable->name, "which is of type", getDataTypeName(variable->tipo->datatype), " but referencing it as array.");
        exit(-1);
    }else{
      //variable->ref== NULL
      //OK - variabile normale
      return 1;
    }
  }
  if(variable->tipo->datatype == dt_array ){

    //altrimenti è un array, devo controllare gli indici ai quali accedo
    //devo controllare anche se è un array di tipi base o meno
   return checkArrayAcces(variable->tipo->value.a,variable->ref);

  }

  printf("%s\n", "errore non gestito in typeChecking");
  exit(-1);
}

/**
  evaluates right hand side of an expression
*/
int Revaluate(LRhand * expr){

    switch (expr->type) {
      case type_VAR:
        {
          symrec * var = expr->value.rec;
          if(var->ref == NULL){
              return var->tipo->value.b->value.i;
          }
          else{
              //sfrutto arrayElement - mi faccio dare l'address
              int * addr = arrayElement(var->tipo->value.a, var->ref);
              return *addr;
          }
        }

        break;
      case type_NUM:
        {
          int n = expr->value.num;
          return n;
        }
        break;
      case type_STRUCT:
        {
          //once here all the control are already done
          //we have to take pointer location and return its value
          //in the LRHand struct we save the struct as rec and the accessed value
          //as name, notice that we can not nest in this case structs calling structs
          //our structures are far from being simple, with a more simple architecture
          //we could play around with this a bit
          symrec s = *getsymStruct(expr->member.name->name,
                                  expr->value.rec->tipo->value.r->tabella);
          //suppose we do not declare array, even if possible
          //so that we have only basic types eg int
          return s.tipo->value.b->value.i;
        }
        break;
      default:
        printf("new LR_TYPE added and not managed in R evaluation function");
        exit(-1);
    }
    printf("%s %u\n", "Right hand operator\nnot able to evaluate expression", expr->type);
    exit(0);

}

int * Levaluate(LRhand * expr){

  switch (expr->type) {
    case type_VAR:
      {
        //i controlli sull'accesso all'array o alla variabile sono gia stati fatti
        symrec * var = (symrec*)malloc(sizeof(symrec));
        var = expr->value.rec;
        if(var->ref == NULL){
            return &(var->tipo->value.b->value.i);
        }
        else{
            return arrayElement(var->tipo->value.a, var->ref);
        }
      }

      break;
    case type_NUM:
      {
        printf("%s\n", "not able to assign value to a constant");
        exit(-1);
      }
      break;
    case type_STRUCT:
      {
        symrec s = *getsymStruct(expr->member.name->name,
                                 expr->value.rec->tipo->value.r->tabella);
        return &(s.tipo->value.b->value.i);
      }
      break;
    default:
      printf("new LR_TYPE added and not managed in R evaluation function");
      exit(-1);
  }


    printf("%s %u\n", "Left hand operator\nnot able to evaluate expression", expr->type);
    exit(0);

}
/**
self evident
*/
int assignment(LRhand * L, LRhand * R){
    //evaluate R and then assign it to L
    double Rvalue = Revaluate(R);
    int *Lvalue = Levaluate(L);
    int v = Rvalue;
    *Lvalue = v;
    return 1;

}


void readTable(symrec * tabella){

  if(tabella == NULL){
  }
  symrec *ptr;

  for (ptr = tabella; ptr != (symrec *) 0; ptr = (symrec *)ptr->next){
    char * tipo;
    switch (ptr->tipo->datatype) {
      case dt_integer:
        tipo = "intero";
        break;
      case dt_float:
        tipo = "float";
        break;
      case dt_array:
        tipo = readArray(ptr->tipo->value.a);
        break;
      case dt_structure:
        tipo = "struttura";
        break;
      default:
        printf("type not correctly managed");
        exit(-2);
    }
  }

}


/**
convalida l'accesso ai membri/variabili di una struct
*/
int validateStructAccess(char const * structName, char const * member){

    symrec *s;
    s = getsym(structName);
    if(s == NULL){
        printf("there is no struct in the environment with name %s\n", structName);
        return 0;
    }
    if(s->tipo->datatype!=dt_structure){
        printf("%s is not a struct\n", structName);
        return 0;
    }
    // so far we checked that we have a struct, is time to check for its property
    symrec * m;
    m = getsymStruct(member, s->tipo->value.r->tabella);
    if(m == NULL){
        printf("There is no %s in %s, are you sure about %s.%s ? \n", member, structName, structName, member);
        return 0;
    }
    //all checks so far are ok
    return 1;

}

//==============================================================================
//==============================================================================
//FUNCTIONS USED BY PARSER.Y
//==============================================================================
//==============================================================================


array * m_array(array * a, int n, const enum data_type datatype){
  indent++;
  array * res = (array*)malloc(sizeof(array));
  if(a == NULL && datatype!= dt_array){
    //BASE CASE
    basic ** basic_type_array = (basic**)malloc(n*sizeof(basic**)); //remove cast, is correct basic**?
    res->datatype = datatype;
    res->size = n;
    for(int i = 0; i<n; i++){
      basic_type_array[i] = (basic*)malloc(sizeof(basic));
      basic_type_array[i]->datatype = datatype;
    }
    res->value.b = basic_type_array;
  }else{
    if(a == NULL){
      printf("unreachable state %s\n", "please check logic for bugs");
      exit(-3);
    }
    array ** array_array = (array**)malloc(n*sizeof(array*)); //remove cast
    res->size = n;
    res->datatype = dt_array;
    if(datatype != dt_array){
      for(int i = 0; i< n; i++){
        array_array[i] = m_array(NULL, a->size, a->datatype); //recursive inductive step
      }
    }else{
      enum data_type dt = a->datatype;
      if(a->value.a[0]->datatype!=dt_array){
          dt = a->value.a[0]->datatype;
      }
      //ASSUME N, GO FOR N+1
      for(int i = 0; i<n; i++){
        array_array[i] = m_array(a->value.a[0], a->size, dt); //recursive inductive step
      }
      array_array[n-1]->datatype = datatype;
      array_array[n-1] = a;
    }
    res->value.a = array_array;

  }
  indent --;
  return res;
}


/*
	INPUT PARAMS:
  <ul>
  <li>array * a</li> un riferimento ad un array - se veniamo dalla riduzione di C -> empty allora questo valore sarà null
  <li>size</li> int, indica la grandezza dell'array da costruire
  <li>default_if_a_null</li> indica il tipo da applicare nel caso in cui l'array sia nullo.. ovvero nel caso in cui si provenga dalla riduzione di C-> empty
  </ul>
*/
array * makeArray(array *a,int size,const enum data_type default_if_a_null){
  array * res;
  enum data_type dt;
  if(a == NULL){
    dt = default_if_a_null;
  }else{
    dt = a->datatype;
  }
  res = m_array(a,size,dt);
  return res;
}

type * composeType(array * a, enum data_type datatype){
	type * res = (type*)malloc(sizeof(type));
	if(a == NULL){
		//è un tipo di base..
		//è dovuto alla costruzione per C -> epsilon dove restituiamo NULL all'elemento sullo stack
	  res->datatype = datatype;
		res->value.b = (basic*)malloc(sizeof(basic));
	  res->value.b->datatype = datatype;
	}else{
		//è proprio un array
		//quindi settiamo direttamente il valore di a per il type
		res->value.a = a;
		res->datatype = dt_array;
	}
	return res;
}

char * readArray(array * a){
  if(a == NULL){
    printf("%s\n", "array is NULL");
    exit(-1);
  }
  char * res;
  switch (a->datatype) {
    case dt_array:
      //value.a[0] serve come campione
      res = concat("array of ", readArray(a->value.a[0]));
      break;
    case dt_integer:
    case dt_float:

      if(a->size>0){
        res = concat("array of basic type: ", getDataTypeName(a->value.b[0]->datatype));
      }
      break;
    case dt_structure:
      printf("%s\n", "case not implemented");
      exit(-1);
      break; //useless
    default:
      printf("%s\n", "a new type has been added, review readArray");
      exit(-1);
      break; //useless
  }
  return res;
}
/**
  controlla se la variabile esiste
  se symboltable è NULL controlla in quella globale, altrimenti in quella passsata
*/
int checkExistance(symrec * symbol, symrec * symboltable){
  //check if variable exists
  int existInEnvironment;
  if(symboltable == NULL){
    existInEnvironment = (getsym(symbol->name) == NULL) ? 0 : 1;
  }else{
    existInEnvironment = (getsymStruct(symbol->name, symboltable)) ? 0 : 1;
  }
  return existInEnvironment;
}
/**
self evident
*/
char* concat(char *s1, char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+1);
    //in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}
