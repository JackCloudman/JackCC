%{
#include <stdio.h>
#include "complejo_cal.h"
#include "Symbol.h"

void yyerror (char *s);
int yylex ();
void warning(char *s, char *t);

%}
%union {
  ComplejoAP val;
  Symbol *sym;
}

%token <val> complexnum
%token <sym> VAR BLTIN INDEF
%type <val> exp asgn

%right '='
%left '+' '-'
%left '*' '/'
%%
list:
  | list'\n'
  | list exp '\n'  { imprimirC($2);}
  | list asgn '\n'
  | list error '\n' {yyerrok;}
  ;
asgn: VAR '=' exp {$$=$1->u.val=$3; $1->type=VAR;}
;
exp:  complexnum        { $$ = $1;  }
      | VAR     {
                  if($1->type == INDEF)
                  printf("Error: '%s' no esta definido\n",$1->name);
                  $$ = $1->u.val;
                }
      | exp '+' exp     { $$ = Complejo_add($1,$3);  }
      | exp '-' exp     { $$ = Complejo_sub($1,$3);  }
      | exp '*' exp     { $$ = Complejo_mul($1,$3);  }
      | exp '/' exp     { $$ = Complejo_div($1,$3);  }
      | '(' exp ')'     { $$ = $2;}
;
%%

#include <stdio.h>
#include <ctype.h>
char *progname;

void main (int argc, char *argv[]){
  progname=argv[0];
    yyparse ();
}
void yyerror (char *s) {
  warning(s, (char *) 0);
}
void warning(char *s, char *t){
  fprintf (stderr, "%s: %s", progname, s);
  if(t)
    fprintf (stderr, " %s", t);
}
