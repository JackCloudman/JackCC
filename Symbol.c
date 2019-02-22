#include "Symbol.h"
static Symbol *symlist=0;    /* tabla de simbolos: lista ligada */

Symbol *lookup(char *s)    /* encontrar s en la tabla de símbolos */
{
Symbol  *sp;
  for (sp = symlist; sp != (Symbol *)0; sp = sp->next){
    if (strcmp(sp->name, s)== 0)
      return sp;
  }
  return 0;      /* 0 ==> no se encontró */
}
/*La funcion install crea la variable en la tabla
de simbolos si esta que esta no existe*/
Symbol *install(char *s,int t,ComplejoAP d)
{
  Symbol *sp;
  char *emalloc();

  sp = (Symbol *) emalloc(sizeof(Symbol));

  sp->name = emalloc(strlen(s)+ 1) ; /* +1 para '\0' */
  strcpy(sp->name, s);

  sp->type = t;
  sp->u.val = d;
  sp->next  =  symlist;   /*  poner al frente de la lista   */
  symlist =  sp;
  return sp;
}

/*   revisar el regreso desde malloc  */
char  *emalloc(unsigned n){
  char *p;
  p =(char*) malloc(n);
  if(p == 0)
    perror("out of memory");
  return p;
}
