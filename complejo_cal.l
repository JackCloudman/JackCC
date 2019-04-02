%option noyywrap
%{
#include <stdio.h>
#include <stdlib.h>
#include "complejo_cal.h"
#include  "Symbol.h"
#include "y.tab.h"

extern Complejo *creaComplejo(double real, double img);
void RmWs(char* str);
extern YYSTYPE yylval;
%}

op [-+*\/()=:;]
ws [ \t]+
digits [0-9]
number (0|[1-9]+{digits}*)\.?{digits}*
im [i]
simplenum {ws}*{ws}*{number}
completenum {ws}*[-]*{ws}*{number}{ws}*[+|-]{ws}*{number}{ws}*{im}{ws}*
complexnum simplenum|completenum
VAR [_]*[a-zA-Z][a-zA-Z0-9_]*
ARG \${number}
STRING \"(\\.|[^"\\])*\"
EXIT exit[\(][\)]
%%
> {return GT;}
>= {return GE;}
\< {return LT;}
\<= {return LE;}
== {return EQ;}
!= {return NE;}
not {return NOT;}
or {return OR;}
and {return AND;}
if {return IF;}
while {return WHILE;}
else {return ELSE;}
print {return PRINT;}
for {return FOR;}
def {return FUNC;}
input {return INPUT;}
proc {return PROC;}
return {return RETURN;}
{ARG} {int n=0;sscanf(yytext,"$%d",&n);yylval.narg =n;return ARG;}
{STRING} {
    String s = makeString(yytext);
    yylval.sym = installString("",STRING,s);
    return STRING;
  }
{completenum} {
  double r, im;
  RmWs(yytext);
  sscanf(yytext,"%lf %lf",&r,&im);
  yylval.sym = installComplejo("",complexnum,creaComplejo(r,im));
  return complexnum;}
{simplenum} {
  double r;
  RmWs(yytext);
  sscanf(yytext,"%lf",&r);
  yylval.sym = installComplejo("",complexnum,creaComplejo(r,0));
  return complexnum;}
{VAR} {
  Symbol *s;
  if((s=lookup(yytext))==0)
    s = installComplejo(yytext,INDEF,creaComplejo(0,0));
  yylval.sym = s;
  return s->type==INDEF?VAR:s->type;
}
^\n$ {return *yytext;}
{op} |
\n {return *yytext;}
{ws} { /* Do nothing */ }
[\{|\}|\[|\]|,] {return *yytext;}
. { printf("%c",*yytext);}
{EXIT} {return EXIT;}
%%

/* function provided to student to remove */
/* all the whitespaces from a string.     */
/* input :       a string of chars        */
/* output:       nothing                  */
/* side effect:  whitespace in the        */
/*               original string removed  */
/* return value: none                     */
void RmWs(char* str) {
  int i = 0, j = 0;
  char temp[strlen(str) + 1];
  strcpy(temp, str);
  while (temp[i] != '\0') {
    while (temp[i] == ' ')
      i++;
    str[j] = temp[i];
    i++;
    j++;
  }
 str[j] = '\0';
}
