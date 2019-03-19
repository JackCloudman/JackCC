#ifndef __LIST_H__
#define __LIST_H__
#include <stdio.h>
#include <stdlib.h>
#include "complejo_cal.h"
typedef struct List{
  union {
    ComplejoAP val;
  }u;
  struct List* next;
  struct List* prev;
}List;
void printList(List*);
List* Listappend(List*,ComplejoAP);
List* Listinsert(List*,ComplejoAP);
List* Listmerge(List*,List*);
ComplejoAP* getElement(List*,int);
List* getListrange(List*,int*,int*);
int getListsize(List*);
List* getListElement(List*,int);
#endif
