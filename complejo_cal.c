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
char* Complejo_to_String(ComplejoAP c){
  char* s = (char*)malloc(sizeof(char)*100);
  gcvt(c->real, 6, s);
  if(c->img != 0){
    char* img = (char*)malloc(sizeof(char)*50);
    img = gcvt(c->img, 6, img);
    if(c->img>0)
      strcat(s,"+");
    strcat(s,img);
    strcat(s,"i");
  }
  return s;

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
