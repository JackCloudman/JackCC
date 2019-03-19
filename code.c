#include "Symbol.h"
#include "y.tab.h"

extern void execerror(char *s, char *t);
void execute(Inst* p);
#define NSTACK  256
static  Datum  stack[NSTACK];  /* la pila */
static  Datum   *stackp;       /* siguiente lugar libre en la pila */
#define NPROG   2000
Inst    prog[NPROG];    /* la máquina */
Inst    *progp;         /* siguiente lugar libre para la generación de código */
Inst    *pc;	/* contador de programa durante la ejecución */
int error = 0;

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
void emptypush(){
  Datum d;
  d.val = 0;
  push(d);
}
void constStringpush( ){ /* meter una complexnum a la pila  */
  Datum d;
  d.s  =  ((Symbol  *)*pc++)->u.s;
  push(d);
}

void varpush(){/* meter una variable a la pila   */
  Datum d;
  d.sym  =  (Symbol   *)(*pc++);
  push(d);
}
void whilecode(){
  Datum d;
  Inst *savepc = pc;
  execute(savepc+2);
  d = pop();
  while(d.val->img!=0 || d.val->real!=0){
    execute(*((Inst  **)(savepc)));
    execute(savepc+2);
    d = pop();
  }
  pc = *((Inst  **)(savepc+1));
}
void ifcode(){
  Datum d;
  Inst  *savepc  = pc;	/* parte then */
  execute(savepc+3);	/*  condición   */
  d  =  pop();
  if(d.val->img||d.val->real)
    execute(*((Inst   **)(savepc)));
  else if(*((Inst  **)(savepc+1)))   /*  ¿parte else?   */
    execute(*(( Inst  **) (savepc+1)));
  pc  =  *((Inst  **)(savepc+2));	/*  siguiente proposición   */
}

void eval( ){ /*  evaluar una variable en la pila   */
  Datum  d;
  d   =  pop();
  if   (d.sym->type   ==   INDEF){
    execerror("Error, variable no definida: ",
              d.sym->name);
    error = 1;
  }
  d.val   =  d.sym->u.val; push(d);
}
void evalS( ){ /*  evaluar una variable en la pila   */
  Datum  d;
  d   =  pop();
  if   (d.sym->type   ==   INDEF){
    execerror("Error, variable no definida: ",
              d.sym->name);
    error = 1;
  }
  d.s = d.sym->u.s; push(d);
}
void evalA( ){ /*  evaluar una variable en la pila   */
  Datum  d;
  d   =  pop();
  if   (d.sym->type   ==   INDEF){
    execerror("Error, variable no definida: ",
              d.sym->name);
    error = 1;
  }
  d.l = d.sym->u.l; push(d);
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
  if (d2.val->img == 0.0 && d2.val->real == 0.0){
    execerror("division by zero", (char *)0);
    error = 1;
  }
  d1 = pop();
  d1.val = Complejo_div(d1.val,d2.val);
  push(d1);
}

void negate()
{
Datum d;
d = pop();
d.val->img = -d.val->img;
d.val->real  = -d.val->real;
push(d);
}
/* OPERACIONES DE COMPARACION */
void gt() {
  Datum d1,  d2;
  d2 = pop();
  d1 = pop();
  d1.val  = creaComplejo(Complejo_abs(d1.val)->real > Complejo_abs(d2.val)->real,0);
  push(d1);
}

void lt(){
  Datum d1,  d2;
  d2 = pop();
  d1 = pop();
  d1.val  =  creaComplejo(Complejo_abs(d1.val)->real < Complejo_abs(d2.val)->real,0);
  push(d1);
}

void ge( ) {
  Datum d1,  d2;
  d2  = pop();
  d1  = pop();
  if(!(Complejo_abs(d1.val)->real > Complejo_abs(d2.val)->real)){ //Si es mayor crea el complejo
    d1.val = creaComplejo(d2.val->real==d1.val->real && d1.val->img==d2.val->img,0);//Si no es mayor, quiza es igual
  }else{
    d1.val = creaComplejo(1,0);
  }
  push(d1) ;
}

void le() {
  Datum d1,  d2;
  d2   =  pop();
  d1   =  pop();
  if(!(Complejo_abs(d1.val)->real < Complejo_abs(d2.val)->real)){
    d1.val = creaComplejo(d2.val->real==d1.val->real && d1.val->img==d2.val->img,0);
  }else{
    d1.val = creaComplejo(1,0);
  }
  push(d1);
}

void eq( ) {
  Datum d1,  d2;
  d2  = pop();
  d1  = pop();
  d1.val = creaComplejo(d2.val->real==d1.val->real && d1.val->img==d2.val->img,0);
  push(d1);
}

void ne(){
  Datum d1, d2;
  d2 = pop();
  d1 = pop();
  d1.val = creaComplejo(d2.val->real!=d1.val->real || d1.val->img!=d2.val->img,0);
  push(d1);
}

void and(){
  Datum d1,   d2;
  d2   = pop();
  d1   = pop();
  d1.val = creaComplejo((Complejo_abs(d1.val)->real!=0) && (Complejo_abs(d2.val)->real!=0),0);
  push(d1);
}

void or(){
  Datum d1, d2;
  d2 = pop();
  d1 = pop();
  d1.val = creaComplejo(Complejo_abs(d1.val)->real!=0 || Complejo_abs(d2.val)->real!=0,0);
  push(d1);
}

void not( ){
  Datum d;
  d = pop();
  d.val = creaComplejo(Complejo_abs(d.val)->real==0,0);
  push(d);
}
/* ===== */
void assign( ){
  Datum d1, d2;
  d1 = pop();
  d2 = pop();
  if (d1.sym->type != VAR && d1.sym->type!=VARS && d1.sym->type!=VARA && d1.sym->type != INDEF){
    execerror("assignment to non-variable", d1.sym->name);
    error = 1;
  }
  d1.sym->u.val = d2.val;
  if(!error)
    d1.sym->type = VAR;
  push(d2);
}
void assignS( ){
  Datum d1, d2;
  d1 = pop();
  d2 = pop();
  if (d1.sym->type != VAR && d1.sym->type!=VARS && d1.sym->type!=VARA && d1.sym->type != INDEF){
    execerror("assignment to non-variable", d1.sym->name);
    error = 1;
  }
  d1.sym->u.s = d2.s;
  if(!error)
    d1.sym->type = VARS;
  push(d2);
}
void assignA(){
  Datum d1, d2;
  d1 = pop();
  d2 = pop();
  if (d1.sym->type != VAR && d1.sym->type!=VARS && d1.sym->type!=VARA && d1.sym->type != INDEF){
    execerror("assignment to non-variable", d1.sym->name);
    error = 1;
  }
  d1.sym->u.l = d2.l;
  if(!error)
    d1.sym->type = VARA;
  push(d2);
}
void print( ){
  Datum d;
  char c = '\n';
  d = pop();
  if(!error)
    imprimirC(d.val,&c);
  error = 0;
}

void bltin( )/*  evaluar un predefinido en el tope de la pila  */
{
  Datum d;
  d  =  pop();
  d.val  =   (*(ComplejoAP   (*)())(*pc++))(d.val);
  push(d);
}
void printS( ){
  Datum d;
  d = pop();
  if(!error)
    printf("'%s'\n",d.s);
  error = 0;
}
void flip(){
  Datum d1,d2;
  d2 = pop();
  d1 = pop();
  push(d2);
  push(d1);
}
void addS( ){
  Datum d1, d2;
  String d = 0;
  d2 = pop();
  d1 = pop();

  d = (String)malloc(sizeof(char)*(strlen(d1.s)+strlen(d2.s)+1));
  strcpy(d,d1.s);
  strcat(d,d2.s);
  d1.s = d;
  push(d1);
}
void convertS(){
  Datum d = pop();
  d.s = Complejo_to_String(d.val);
  push(d);
}
void makeArray(){
  Datum d;
  List* l = 0;
  Datum* savestackp = stackp;
  execute(pc);
  while(stackp > savestackp){
    d = pop();
    l = Listinsert(l,d.val);
  }
  d.l = l;
  push(d);
  *pc++;
}
void getSubArray(){
  Datum start,end,lista;
  int *s = 0;
  int *e = 0;
  List *aux = 0;

  end = pop();
  start = pop();
  lista = pop();

  if(end.val !=0){
    if(end.val->img!=0)
      execerror("Only int number!", (char *) 0);
    e = (int*)malloc(sizeof(int));
    *e = end.val->real;
  }
  if(start.val!=0){
    if(start.val->img!=0)
      execerror("Only int number!", (char *) 0);
    s = (int*)malloc(sizeof(int));
    *s = start.val->real;
  }
  aux = getListrange(lista.l,s,e);
  lista.l = aux;
  push(lista);
}
void MergeArray(){
  Datum d2,d1;
  d2 = pop();
  d1 = pop();
  d1.l = Listmerge(d1.l,d2.l);
  push(d1);
}
void printArray(){
  Datum d;
  d = pop();
  printList(d.l);
}
void aArray(){
  Datum d1,d2;
  ComplejoAP* c = 0;
  d2 = pop();
  if(d2.val->img!=0)
    execerror("Only int number!", (char *) 0);
  d1 = pop();
  c = getElement(d1.l,d2.val->real);
  if(c==0){
    execerror("IndexError: list index out of range", (char *) 0);
    return;
  }
  d1.val = *c;
  push(d1);
}
void ChangeValue(){
  Datum nd,index,a;//New dato,index, array
  ComplejoAP* c = 0;
  nd = pop();
  index = pop();
  a = pop(); //Save array
  if(index.val->img!=0)
    execerror("Only int number!", (char *) 0);
  c = getElement(a.l,index.val->real);
  if(c==0){
    execerror("IndexError: list index out of range", (char *) 0);
    return;
  }
  (*c) = nd.val;
  push(a);
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
