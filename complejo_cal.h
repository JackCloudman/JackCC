#ifndef __COMPLEJO_h__
#define __COMPLEJO_h__
  #include<stdio.h>
  #include<stdlib.h>
  #include<math.h>
  struct complejo {
    double real, img;
  };

  typedef struct complejo Complejo;
  typedef struct complejo *ComplejoAP;

  ComplejoAP creaComplejo(double real, double img);
  ComplejoAP Complejo_add(Complejo *, Complejo *);
  ComplejoAP Complejo_sub(Complejo *, Complejo *);
  ComplejoAP Complejo_mul(Complejo *, Complejo *);
  ComplejoAP Complejo_div(Complejo *, Complejo *);
  ComplejoAP Complejo_sen(Complejo *);
  ComplejoAP Complejo_cos(Complejo *);
  ComplejoAP Complejo_abs(Complejo*);
  void imprimirC(Complejo *c);
#endif
