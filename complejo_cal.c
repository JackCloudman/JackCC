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
ComplejoAP Complejo_abs(ComplejoAP c){
  return creaComplejo(sqrt(c->real+c->img),0);
}

void imprimirC(ComplejoAP c){
  if(c==0)
    return;
   if(c->img != 0)
      printf("%f%+fi\n", c->real, c->img);
   else
      printf("%f\n", c->real);
}
