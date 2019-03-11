#ifndef __SYMBOL_H__
#define __SYMBOL_H__
#include "complejo_cal.h"
#include <string.h>
/*entrada a tabla de simbolos,
es una lista simplemente ligada*/
typedef struct Symbol {
  char   *name;
  short   type;   /* VAR, BLTIN, UNDEF */

  union {
    ComplejoAP val;	       /* si es VAR */
    ComplejoAP (*ptr)();      /* sí es BLTIN */
  } u;

  struct Symbol   *next;  /* para ligarse a otro */
} Symbol;

Symbol *install(char *s,int t, ComplejoAP d), *lookup(char *s);

typedef union Datum {   /* tipo de la pila del intérprete */
  ComplejoAP  val;
  Symbol  *sym; } Datum;

extern Datum pop();

typedef void (*Inst)();  /* instrucción de máquina */

#define STOP    (Inst) 0
extern  Inst prog[];
extern  void eval(), addc(), subc(), mulc(), divc(),negate(),whilecode(),ifcode();
extern void gt(),ge(),lt(),le(),eq(),ne(),and(),or(),not();
extern void  assign(), bltin(), varpush(), constpush(), print();

#endif
