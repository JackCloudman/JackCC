#ifndef __COMPLEJO_h__
#define __COMPLEJO_h__
  #include<stdio.h>
  #include<stdlib.h>
  #include<math.h>
  #include <string.h>
  #include <ctype.h>
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
  ComplejoAP Complejo_tan(Complejo *);
  ComplejoAP Complejo_abs(Complejo*);
  char * Complejo_to_String(ComplejoAP);
  ComplejoAP String_to_Complejo(char*);
  ComplejoAP Complejo_real(ComplejoAP);
  ComplejoAP Complejo_img(ComplejoAP);
  void imprimirC(Complejo *c,char*);
#endif
