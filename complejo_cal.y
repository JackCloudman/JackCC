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
extern void execute();
extern void initcode();
extern Inst *progp;
%}
%union {
  Symbol *sym;
  Inst *inst;
}

%token <sym> complexnum VAR BLTIN INDEF EXIT WHILE IF ELSE PRINT
%type <inst> stmt asgn exp stmtlist cond while if end

%right '='
%left OR
%left AND
%left GT GE LT LE EQ NE
%left '+' '-'
%left '*' '/'
%left UNARYMINUS NOT
%%
list:
  | list'\n'
  | list asgn '\n' {code2((Inst)pop,STOP);return 1;}
  | list stmt '\n' {code(STOP);return 1;}
  | list exp '\n'  { code2(print,STOP);return 1;}
  | list error '\n' {initcode();printf(">>> ");yyerrok;}
  ;
asgn: VAR '=' exp {code3(varpush,(Inst)$1,assign);}
  ;
stmt: exp {code((Inst)pop);}
    | PRINT exp { code(print); $$ = $2;}
    | while cond stmt end {
        ($1)[1] = (Inst)$3;
        ($1)[2] = (Inst)$4;
      }
    | if cond stmt end {
        ($1)[1] = (Inst)$3;
        ($1)[3] = (Inst)$4;
      }
    | if cond stmt end ELSE stmt end {
        ($1)[1] = (Inst)$3;
        ($1)[2] = (Inst)$6;
        ($1)[3] = (Inst)$7;
      }
    | '{' stmtlist '}' {$$=$2;}
    ;
cond: exp  {code(STOP);$$=$1;}
  ;
while: WHILE {$$= code3(whilecode,STOP,STOP);}
  ;
if: IF {$$=code(ifcode);code3(STOP,STOP,STOP);}
  ;
end:  {code(STOP);$$=progp;}
  ;
stmtlist: {$$=progp;}
      | stmtlist '\n' {printf("... ");}
      | stmtlist stmt
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
      |'-' exp %prec UNARYMINUS {code(negate);}
      | exp GT exp {code(gt);}
      | exp GE exp {code(ge);}
      | exp LT exp {code(lt);}
      | exp LE exp {code(le);}
      | exp EQ exp {code(eq);}
      | exp NE exp {code(ne);}
      | exp AND exp {code(and);}
      | exp OR exp {code(or);}
      | NOT exp {$$=$2;code(not);}
      | EXIT {code(exit);}
  ;
%%

#include <stdio.h>
#include <ctype.h>
char *progname;

void main (int argc, char *argv[]){
  progname=argv[0];
  init();
  printf("Jack Complex Calculator v1.5\n[GCC 8.2.1 20181127]\n>>> ");
  for(initcode(); yyparse (); initcode()){
    execute(prog);
    printf(">>> ");
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
