#ifndef __COMPLEJO_h__
#define __COMPLEJO_h__
  #include<stdio.h>
  #include<stdlib.h>
  struct complejo {
    double real, img;
  };

  typedef struct complejo Complejo;
  typedef struct complejo *ComplejoAP;

  Complejo *creaComplejo(int real, int img);
  Complejo *Complejo_add(Complejo *, Complejo *);
  Complejo *Complejo_sub(Complejo *, Complejo *);
  Complejo *Complejo_mul(Complejo *, Complejo *);
  Complejo *Complejo_div(Complejo *, Complejo *);
  void imprimirC(Complejo *c);
#endif
