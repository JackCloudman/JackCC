#include "complejo_cal.h"
ComplejoAP creaComplejo(double real, double img){
   ComplejoAP nvo = (Complejo*)malloc(sizeof(Complejo));
   nvo -> real = real;
   nvo -> img = img;
   return nvo;
}
ComplejoAP Complejo_add(ComplejoAP c1, ComplejoAP c2){
  return creaComplejo(c1->real + c2->real, c1->img + c2->img);
}
ComplejoAP Complejo_sub(ComplejoAP c1, ComplejoAP c2){
  return creaComplejo(c1->real - c2->real, c1->img - c2->img);
}
ComplejoAP Complejo_mul(ComplejoAP c1, ComplejoAP c2){
  return creaComplejo( c1->real*c2->real - c1->img*c2->img,
                       c1->img*c2->real + c1->real*c2->img);
}
ComplejoAP Complejo_div(ComplejoAP c1, ComplejoAP c2){
   double d = c2->real*c2->real + c2->img*c2->img;
   return creaComplejo( (c1->real*c2->real + c1->img*c2->img) / d,
                        (c1->img*c2->real - c1->real*c2->img) / d);
}
/*Funciones matemáticas "avanzadas"*/
ComplejoAP Complejo_sen(ComplejoAP c){
  return creaComplejo(sin(c->real)*cosh(c->img),cos(c->real)*sinh(c->img));
}
ComplejoAP Complejo_cos(ComplejoAP c){
  return creaComplejo(cos(c->real)*cosh(c->img),-(sin(c->real)*sinh(c->img)));
}
ComplejoAP Complejo_log(ComplejoAP c){

   return creaComplejo(0.5*log(((c->real)*(c->real))+((c->img)*(c->img))),atan((c->img)/(c->real)));
}
ComplejoAP Complejo_tan(ComplejoAP c){
  return Complejo_div(Complejo_sen(c),Complejo_cos(c));
}
ComplejoAP Complejo_abs(ComplejoAP c){
  return creaComplejo(sqrt(c->real+c->img),0);
}
ComplejoAP Complejo_real(ComplejoAP c){
  return creaComplejo(c->real,0);
}
ComplejoAP Complejo_img(ComplejoAP c){
  return creaComplejo(0,c->img);
}
char* Complejo_to_String(ComplejoAP c){
  char* s = (char*)malloc(sizeof(char)*30);
  int j;
  j = sprintf(s,"%f",c->real);
  if(c->img != 0){
    if(c->img>0)
      j += sprintf(s+j,"+%fi",c->img);
    else
      j += sprintf(s+j,"%fi",c->img);
  }
  s[j]='\0';
  return s;

}
ComplejoAP String_to_Complejo(char* buffer/*Sin espacios :P*/){
  double real=0,img=0;
  char op1=0,op2;
  int j;
  if(buffer[0]=='.'||isdigit(buffer[0])||buffer[0]=='-'){
    if(buffer[0]=='-'){
      j = sscanf(buffer,"%c%lf%c%lfi",&op1,&real,&op2,&img);
    }
    else j = sscanf(buffer,"%lf%c%lfi",&real,&op2,&img);
    if(j==1||j==2){
      if(op1!='-'&&op1!=0)return 0;
      if(op1=='-')
        real=-real;
      return creaComplejo(real,0);
    }
    if(j==4||j==3){
      if(buffer[strlen(buffer)-1]!='i') return 0;
      if((op1!='-'&&op1!=0)||(op2!='-'&&op2!='+')) return 0;
      if(op1=='-')
        real=-real;
      if(op2=='-')
        img = -img;
      return creaComplejo(real,img);
    }
  }
  return 0;
}
void imprimirC(ComplejoAP c,char* end){
  if(end == 0)
    *end = '\n';
  if(c==0)
    return;
   if(c->img != 0)
      printf("%f%+fi%c", c->real, c->img,*end);
   else
      printf("%f%c", c->real,*end);
}
