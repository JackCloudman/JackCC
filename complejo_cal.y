%{
#include <stdio.h>
#include "complejo_cal.h"
#include "Symbol.h"
extern void *code(Inst);
#define code2(c1,c2) code(c1); code(c2);
#define code3(c1,c2,c3) code(c1); code(c2); code(c3);

void yyerror (char *s);
int yylex ();
void warning(char *s, char *t);
void execerror(char *s, char *t);

extern void init();
%}
%union {
  Symbol *sym;
  Inst *inst;
}

%token <sym> complexnum VAR BLTIN INDEF EXIT

%right '='
%left '+' '-'
%left '*' '/'
%%
list:
  | list'\n'
  | list asgn '\n' {code2(pop,STOP);printf(">>> "); return 1;}
  | list exp '\n'  { code2(print,STOP);return 1;}
  | list error '\n' {yyerrok;printf(">>> ");}
  ;
asgn: VAR '=' exp {code3(varpush,(Inst)$1,assign);}
;
exp:  complexnum  { code2(constpush,(Inst)$1);}
      | VAR       {code3(varpush,(Inst)$1,eval);}
      | asgn
      | exp '+' exp     { code(addc);  }
      | exp '-' exp     { code(subc);}
      | exp '*' exp     { code(mulc);}
      | exp '/' exp     { code(divc);}
      | '(' exp ')' {}
      |BLTIN  '(' exp ')' {code2(bltin,(Inst)$1->u.ptr);}
      | EXIT {exit(0);}
;
%%

#include <stdio.h>
#include <ctype.h>
char *progname;

void main (int argc, char *argv[]){
  progname=argv[0];
  init();
  printf("Jack Complex Calculator v1.4\n[GCC 8.2.1 20181127]\n>>> ");
  for(initcode(); yyparse (); initcode()){
    execute(prog);
  }
}
void yyerror (char *s) {
  warning(s, (char *) 0);
}
void warning(char *s, char *t){
  fprintf (stderr, "%s",s);
  if(t)
    fprintf (stderr, "%s", t);
  fprintf(stderr,"\n");
}
void execerror(char *s, char *t)
{
	warning(s, t);
}
