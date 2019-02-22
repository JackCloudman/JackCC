%{
#include <stdio.h>
#include "complejo_cal.h"

void yyerror (char *s);
int yylex ();
void warning(char *s, char *t);
%}
%token complexnum
%left '+' '-'
%left '*' '/'
%%
list:
	| list'\n'
        | list exp '\n'  { imprimirC($2);}
	;
exp:      complexnum          { $$ = $1;  }
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
