#include "Symbol.h"
#include "y.tab.h"

extern void execerror(char *s, char *t);
#define NSTACK  256
static  Datum  stack[NSTACK];  /* la pila */
static  Datum   *stackp;       /* siguiente lugar libre en la pila */
#define NPROG   2000
Inst    prog[NPROG];    /* la máquina */
Inst    *progp;         /* siguiente lugar libre para la generación de código */
Inst    *pc;	/* contador de programa durante la ejecución */


void initcode(){ /* inicialización para la generación de código */
stackp = stack;
progp = prog;
}

void push(d)	/*  meter d en la pila  */
Datum d;
{
if (stackp >= &stack[NSTACK])
execerror("stack overflow", (char *) 0);
*stackp++ = d;
}

Datum pop( ){ /* sacar y retornar de la pila el elemento del tope */
  if (stackp <= stack)
    execerror("stack underflow", (char *) 0);
  return  *--stackp;
}


void constpush( ){ /* meter una complexnum a la pila  */
  Datum d;
  d.val  =  ((Symbol  *)*pc++)->u.val;
  push(d);
}

void varpush(){/* meter una variable a la pila   */
  Datum d;
  d.sym  =  (Symbol   *)(*pc++);
  push(d);
}

void eval( ){ /*  evaluar una variable en la pila   */
  Datum  d;
  d   =  pop();
  if   (d.sym->type   ==   INDEF)
    execerror("Error, variable no definida: ",
  d.sym->name);
  d.val   =  d.sym->u.val; push(d);
}

void addc( ){
  Datum d1,   d2;
  d2 = pop();
  d1 = pop();
  d1.val = Complejo_add(d2.val,d1.val);
  push(d1);
}

void subc(){
Datum d1,  d2;
  d2 = pop();
  d1 = pop();
  d1.val = Complejo_sub(d1.val,d2.val);
  push(d1);
}

void mulc(){
  Datum d1, d2;
  d2 = pop();
  d1 = pop();
  d1.val = Complejo_mul(d1.val,d2.val);
  push(d1);
}


void divc( ){
  Datum d1, d2;
  d2 = pop();
  if (d2.val->img == 0.0 && d2.val->real == 0.0)
    execerror("division by zero", (char *)0);
  d1 = pop();
  d1.val = Complejo_div(d1.val,d2.val);
  push(d1);
}

/*negate()
{
Datum d;
d = pop();
d.val  =  -d.val;
push(d);
}

power()
{
Datum d1, d2;
extern double Pow();
d2 = pop();
d1 = pop();
d1.val = Pow(d1.val, d2.val);
push(d1);
}*/


void assign( ){
  Datum d1, d2;
  d1 = pop();
  d2 = pop();
  if (d1.sym->type != VAR && d1.sym->type != INDEF)
    execerror("assignment to non-variable", d1.sym->name);
  d1.sym->u.val = d2.val;
  d1.sym->type = VAR;
  push(d2);
}

void print( ){
  Datum d;
  d = pop();
  imprimirC(d.val);
  printf(">>> ");
}

void bltin( )/*  evaluar un predefinido en el tope de la pila  */
{
  Datum d;
  d  =  pop();
  d.val  =   (*(ComplejoAP   (*)())(*pc++))(d.val);
  push(d);
}


Inst   *code(Inst f) /*   instalar una instrucción u operando   */
{
Inst *oprogp = progp;
	if (progp >= &prog [ NPROG ])
		execerror("program too big", (char *) 0);
	*progp++ = f;
	return oprogp;
}

void execute(Inst* p)	/*   ejecución con la máquina   */
{
for (pc  =  p;   *pc != STOP; )
	(*(*pc++))();
}
