
/** calculator calc.y
 * inspire by originial source, Tom Niemann @ epaperpress.com/lexandyacc
 * revision Lorenzo Massimo Gramola (2014)
 * complete review Lorenzo Massimo Gramola (2015)

 */

#ifndef HEADER_H
#define HEADER_H

#define true 1
#define false 0

//error DEFINITIONS
#define NOT_IMPLEMENTED_YET -1
#define OUT_OF_MEMORY -2
#define BUGGY_THE_CLOWN -3
#define NOT_ALLOWED -3
#define NAME_ALREADY_IN_USE -4
#define NO_SUCH_VARIABLE -5
#define NO_SUCH_PROCEDURE -6
#define FORM_LIST_EXCEEDED -7
#define ARGS_LIST_EXCEEDED -8



typedef enum {const_type, identifier_type, expression_type, routine_type, identifier_declaration } nodeType; /* used in the struct nodeType to define the type of node*/
typedef enum {undef, basic_int_value, basic_float_value, basic_boolean_value } basicType;
typedef enum {complex_array, complex_structure} complexType;
typedef enum {basic_dataType, complex_dataType, procedure_type, no_op} dataType;
typedef enum {function, procedure} routineType;
typedef enum {routine_list} listContent;

typedef int bool;

typedef struct basic{
  basicType type;
  union{
     int i;
     float f;
     bool b;
  };
}basic;

typedef struct array{
  basicType type;
  union{//farlocco, da sistemare
     int i;
     bool b;
  };
}array;

typedef struct complex{
  complexType type;
  union{
     int i;
     bool b;
  };
}complex;

typedef struct type{
  dataType dt;
  int size;
  union{
    basicType bt;
    complexType ct;
  }typeValue;
  struct type * t; //is the next type
}type;

/*
a symbol in the symbol table
 - name is the name of the symbol
 - type indicates the data type stored by the symbol
*/
typedef struct symrec
{
	char * name;
  dataType type;
  basicType bType;
  complexType cType;
  bool isCostant;
  union{
    int i;

    bool b;
    complex c;
  }*value;
	struct symrec *next;
} symrec;

/* constants are stored in constant node type union */
typedef struct constant{
  basicType type;
  union{
    int int_value;
    bool bool_value;
  } value;
} constant;

/* identifier for node union*/
typedef struct identifier{
  char * name;
}identifier;

typedef struct {
    int operator;                   /* operator */
    int noperands;                  /* number of operands */
    struct treeNode **op;	          /* operands */
} node;

typedef struct declarationNode{
    char * name;
    bool isCostant;
    type * t;
    struct treeNode * expr;
}declarationNode;

typedef struct routineNode{
   char * name;
   struct actual * args;
}routineNode;

typedef struct treeNode{
  nodeType type;              /* type of node, tells us the value contained in the union */
  union {
   constant con;        /* constants */
   identifier id;       /* identifiers */
   node expr;           /*can be the same under the name of operator*/
   declarationNode dec;
   routineNode routine; /*routine or procedure*/
  } value;
} treeNode;


/*--------------------------------.
| PARSER AUXILIARY FUNCTIONS      |
`--------------------------------*/
//initialize a node
treeNode * init();

//expressions
treeNode * constantNode(const basicType, ... );
treeNode * identifierNode(const char *);
treeNode * opr(int oper, int nops, ...);
treeNode * fpCall(const char *, struct actual *);
treeNode * varDec(char *, bool constant, type * dataType, ...);
/*--------------------------------.
| SYMBOL TABLE FUNCTIONS          |
`--------------------------------*/
symrec * getSym(char const *, symrec *);
symrec * createSym(char const *, symrec **);
symrec * putSym(char const *, symrec **);

/*--------------------------------.
| ARRAY DECLARATION               |
`--------------------------------*/

type * arrayDec(int, type *, basicType);
type * basicDec(basicType);

/*--------------------------------.
|    FORM STRUCT                  |
`--------------------------------*/

typedef struct form{
    char * name;
    bool byref;
    dataType type;
    union{
      basicType bt;
      complexType ct;
    };
    struct form * next;
}form;

//adesso usato solamente per il basic type..
form * newParam(const char * , bool byref, dataType ,...);
form * formList(form * , form ** );
int formLength(form *);
void printFormList(form * );
form * getFormAtIndex(int , form *);
form * formList(form * , form ** );

typedef struct actual{
    treeNode * expr;
    struct actual * next;
}actual;
actual * newActual(treeNode *);
actual * addToActList(actual *, actual **);
int actLength(actual *);
actual * getActualAtIndex(int, actual *);

typedef struct data{
    dataType type;
    union{
      complex c;
      basic b;
    };
}data;

/*--------------------------------.
|    FUNCTION AND PROCEDURES      |
`--------------------------------*/
typedef struct routine{
    char * name;
    routineType type;
    dataType returnType;
    union{
      basicType bt;
      complexType ct;
    };
    struct data * returnValue; //so that if it is NULL is clean
    form * parameters;

    treeNode * statementList;
    symrec * env; //is this one used?? TODO: use it for evaluation or remove it
}routine;

routine * newRoutine(const char *,form *,treeNode *,...);

typedef struct list {
  listContent type;
  union{
    routine * r;
  };
  struct list * next;
}list;

routine * getRoutine(const char *, list *);

list * addToList(routine *, list **);
void printAllProcAndFun(list * rList);
/*--------------------------------.
|    PROGRAM STRUCT               |
`--------------------------------*/
 typedef struct program{
     treeNode * statementList;
     list * routineList;
     struct symrec * symtable;
 }program;


 void executeProgram(program *, symrec **, list * );
 void freeNode(treeNode *);
 /*--------------------------------.
 |    EVALUATION FUCNTIONS         |
 `--------------------------------*/
 data eval(treeNode *p, symrec **, list *);
 data eval_constants(constant, symrec **);
 data eval_expr(node, symrec ** , list *);
 data eval_routine(routineNode , symrec **, list *);
 //for already evaluated data types
 //assigns data to symrec passed, does not use yet the routineList nor the symrec table
 data spec_assignment(symrec *, data , symrec **,list * );
 //general one, evaluates treeNode and assign it to symrec, using the given symrec table
 data assignment(symrec *, treeNode *, symrec **,list * );
 data r_assignment(routine *, treeNode *, symrec **,list * );

 data * dataToDataPointer(data);

 data eval_identifier(identifier, symrec **, list *);
 symrec * getSymbolFromIdentifier(identifier, symrec **);

 void printData(data);
 data operation(int oper, data e1, data e2);
 data negate(data);

 data eval_identifier_declaration(declarationNode, symrec **, list * routineList);



#endif
