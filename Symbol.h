#ifndef __SYMBOL_H__
#define __SYMBOL_H__
#include "complejo_cal.h"
#include "List.h"
#include <string.h>
/*entrada a tabla de simbolos,
es una lista simplemente ligada*/
typedef char* String;
typedef void (*Inst)();  /* instrucción de máquina */
typedef struct Symbol {
  char   *name;
  short   type;   /* VAR, BLTIN, UNDEF */

  union {
    String s;
    ComplejoAP val;	       /* si es VAR */
    ComplejoAP (*ptr)();      /* sí es BLTIN */
    List *l; /*LISTA*/
    /*void     (*defn)();*/   /* FUNCIÓN, PROCEDIMIENTO */
    Inst defn;
  } u;

  struct Symbol   *next;  /* para ligarse a otro */
} Symbol;

Symbol *installComplejo(char *s,int t, ComplejoAP d), *lookup(char *s);
Symbol *installString(char *s,int t, char* d);
char* makeString(char* cadena);

typedef union Datum {   /* tipo de la pila del intérprete */
  List* l;
  ComplejoAP  val;
  String s;
  Symbol  *sym; } Datum;

extern Datum pop();

#define STOP    (Inst) 0
extern  Inst prog[];
extern  void eval(), addc(), subc(), mulc(), divc(),negate(),whilecode(),ifcode(),forcode(),varread();;
extern void gt(),ge(),lt(),le(),eq(),ne(),and(),or(),not();
extern void  assign(), bltin(), varpush(), constpush(), print(),ChangeValue(),emptypush();
extern void convertS(),evalS(),addS(),printS(),printSE(),assignS(),constStringpush(),flip(),printInput();
extern void makeArray(),printArray(),MergeArray(),assignA(),evalA(),aArray(),getSubArray();
extern void call(), arg(), argassign();
extern void funcret(), procret(),define();
#endif
