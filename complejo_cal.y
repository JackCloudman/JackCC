%{
#include <stdio.h>
#include "complejo_cal.h"
#include "Symbol.h"
extern void *code(Inst);
#define code2(c1,c2) code(c1); code(c2);
#define code3(c1,c2,c3) code(c1); code(c2); code(c3);
int indef;//Para saber si un return esta fuera de una funcion
void defnonly( char *s );
void yyerror (char *s);
int yylex ();
void warning(char *s, char *t);
void execerror(char *s, char *t);

extern void init();
extern void execute();
extern void initcode();
extern Inst *progp;
extern FILE *yyin;
extern Inst *pc;
extern Inst   *progbase;
int readfile = 0;
%}
%union {
  Symbol *sym; //Tabla de simbolos
  Inst *inst; //Instruccion de maquina virtual
  String s; //Parte string
  int narg; //Numero de argumentos
}
%token <sym> complexnum VAR VARS VARA BLTIN INDEF EXIT WHILE IF ELSE PRINT STRING FOR
%token <sym> FUNCTION PROCEDURE RETURN FUNC PROC INPUT
%token <narg> ARG
%type <inst> stmt asgn exp stmtlist string asgnS array arraylist initarray asgnA
%type <inst> cond while if end for inc start begin
%type <sym> procname
%type <narg> arglist
%right '='
%left '[' ']'
%left OR
%left AND
%left GT GE LT LE EQ NE
%left '+' '-'
%left '*' '/'
%left ','
%left UNARYMINUS NOT
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%%
list:
  | list'\n'
  | list asgn '\n' {code2((Inst)pop,STOP);return 1;}
  | list asgnS '\n' {code2((Inst)pop,STOP);return 1;}
  | list stmt '\n' {code(STOP);return 1;}
  | list exp '\n'  { code2(print,STOP);return 1;}
  | list string'\n' {code2(printSE,STOP);return 1;}
  | list error '\n' {initcode();printf(">>> ");yyerrok;}
  | list array '\n' {code2(printArray,STOP);return 1;}
  | list asgnA '\n' {code2((Inst)pop,STOP);return 1;}
  | list defn '\n' {if(!readfile)printf(">>> ");}
  ;
initarray: {code(makeArray);}
array: initarray '['arraylist']' {$$=$1;code(STOP);}
  | VARA {$$=code3(varpush,(Inst)$1,evalA);}
  | array'+'array {$$=code(MergeArray);}
  | asgnA {$$=$1;}
  | array '[' index ':' index ']' {$$=$1;code(getSubArray);}
  ;
index: exp {}
  | {code(emptypush);}
  ;
arraylist: arraylist','arraylist {}
  | exp {}
  | {$$=progp;}
  ;
asgnA: VAR '=' array {$$=$3;code3(varpush,(Inst)$1,assignA);}
  |    VARS '=' array {$$=$3;code3(varpush,(Inst)$1,assignA);}
  |    VARA '=' array {$$=$3;code3(varpush,(Inst)$1,assignA);}
  | array '[' exp ']' '=' exp {$$=$6;code(ChangeValue);}
  ;
asgn: VAR '=' exp {$$=$3;code3(varpush,(Inst)$1,assign);}
  |   VARS '=' exp {$$=$3;code3(varpush,(Inst)$1,assign);}
  |   VARA '=' exp {$$=$3;code3(varpush,(Inst)$1,assign);}
  |   ARG '=' exp { defnonly("$"); code2(argassign,(Inst)$1); $$=$3;}
  ;
asgnS: VAR '=' string {$$=$3;code3(varpush,(Inst)$1,assignS);}
  |    VARS '=' string {$$=$3;code3(varpush,(Inst)$1,assignS);}
  |    VARA '=' string {$$=$3;code3(varpush,(Inst)$1,assignS);}
;
string: STRING {$$=code2(constStringpush,(Inst)$1);}
  | string '+' string {code(addS);}
  | string '+' exp {code2(convertS,addS);}
  | exp '+' string {code2(flip,convertS);code2(flip,addS);}
  | VARS {code3(varpush,(Inst)$1,evalS);}
  | asgnS
  ;
stmt: exp {code((Inst)pop);}
    | string {code((Inst)pop);}
    | array {code((Inst)pop);}
    | PRINT exp { code(print); $$ = $2;}
    | PRINT string {code(printS);$$=$2;}
    | PRINT array {code(printArray);$$=$2;}
    | for '('start';'cond';'inc')' stmt end{
       ($1)[1] = (Inst)$9;
       ($1)[2] = (Inst)$10;
       ($1)[3] = (Inst)$5;
       ($1)[4] = (Inst)$7;
    }
    | while cond stmt end {
        ($1)[1] = (Inst)$3;
        ($1)[2] = (Inst)$4;
      }
    | if cond stmt end  %prec LOWER_THAN_ELSE {
        ($1)[1] = (Inst)$3;
        ($1)[3] = (Inst)$4;
      }
    | if cond stmt end ELSE stmt end {
        ($1)[1] = (Inst)$3;
        ($1)[2] = (Inst)$6;
        ($1)[3] = (Inst)$7;
      }
    | '{' stmtlist '}' {$$=$2;}
    | RETURN { defnonly("return"); code(procret); }
    | RETURN exp {defnonly( "return" ); $$ = $2; code(funcret); }
    | PROCEDURE begin '(' arglist ')'{ $$ = $2; code3(call, (Inst)$1, (Inst)$4);}
    ;
defn:    FUNC procname { $2->type=FUNCTION; indef=1; }
          '(' ')' stmt {code(procret); define($2); indef=0; }
    | PROC procname { $2->type=PROCEDURE; indef=1; }
          '(' ')' stmt { code(procret); define($2); indef=0; }
       ;
arglist:  /* nada */   { $$ = 0; }
    | exp                 { $$ = 1; }
    | arglist ',' exp     { $$ = $1 + 1; }
;
procname: VAR
   | FUNCTION
   | PROCEDURE
   ;
start: exp {code(STOP);$$=$1;}
  | {$$=code(STOP);}
;
inc: exp {code(STOP);$$=$1;}
  | {$$=code(STOP);}
;
cond: exp  {code(STOP);$$=$1;}
  ;
while: WHILE {$$= code3(whilecode,STOP,STOP);}
  ;
for: FOR {$$ = code3(forcode,STOP,STOP);code2(STOP,STOP);}
  ;
if: IF {$$=code(ifcode);code3(STOP,STOP,STOP);}
  ;
begin:  /* nada */          { $$ = progp; }
  ;
end:  {code(STOP);$$ = progp;}
  ;
stmtlist: {$$ = progp;}
      | stmtlist '\n' {if(!readfile)printf("... ");}
      | stmtlist stmt
  ;
exp:  complexnum  { $$=code2(constpush,(Inst)$1);}
      | VAR       {$$=code3(varpush,(Inst)$1,eval);}
      | ARG    {   defnonly("$"); $$ = code2(arg, (Inst)$1); }
      | asgn
      | FUNCTION begin '(' arglist ')'
              { $$ = $2; code3(call,(Inst)$1,(Inst)$4); }
      | INPUT'('string')' { code(printInput);$$ =code(varread); }
      | exp '+' exp     { code(addc);  }
      | exp '-' exp     { code(subc);}
      | exp '*' exp     { code(mulc);}
      | exp '/' exp     { code(divc);}
      | '(' exp ')' {}
      |BLTIN  '(' exp ')' {$$=$3;code2(bltin,(Inst)$1->u.ptr);}
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
      | array '[' exp ']' {$$=$1;code(aArray);}
      | EXIT {code(exit);}
  ;
%%

#include <stdio.h>
#include <ctype.h>
#include <signal.h>
#include <setjmp.h>
jmp_buf begin;
char *progname;

int main (int argc, char *argv[]){
  progname=argv[0];
  init();
  if(argc==1){
    printf("Jack Complex Calculator v1.7 \n[GCC 8.2.1 20181127]\n");
    setjmp(begin);
    printf(">>> ");
    for(initcode(); yyparse (); initcode()){
      execute(progbase);
      printf(">>> ");
    }
  }
  if(argc==2){
    readfile = 1;
    yyin = fopen(argv[1],"r");
    for(initcode();yyparse();initcode()){
      execute(progbase);
    }
  }
  return 0;
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
  if(readfile)
    exit(1);
  longjmp(begin, 0);
}
void defnonly( char *s )     /* advertena la si hay definición i legal */
{
if (!indef)
	execerror(s, "used outside definition");
}
