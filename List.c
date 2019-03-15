#include "List.h"
List* Listinsert(List* l,ComplejoAP d){
  List* n = (List*)malloc(sizeof(List));
  n->u.val = d;
  if(l==0){
    n->next = n;
    n->prev = n;
  }else{
    n->next = l;
    l->prev->next = n;
    n->prev = l->prev;
    l->prev = n;
  }
  return n;
}
List* Listappend(List *l,ComplejoAP d){
  List* n = (List*)malloc(sizeof(List));
  n->u.val = d;
  if(l==0){
    n->next = n;
    n->prev = n;
    return n;
  }else{
    n->next = l;
    n->prev = l->prev;
    l->prev->next = n;
    l->prev = n;
  }
  return l;
}
List* Listmerge(List *l1,List *l2){
  List* aux,*n;
  n = 0;
  if(l1!=0){
    aux = l1;
    while(aux->next!=l1){
      n = Listappend(n,aux->u.val);
      aux = aux->next;
    }
    n = Listappend(n,aux->u.val);
  }
  if(l2!=0){
    aux = l2;
    while(aux->next!=l2){
      n = Listappend(n,aux->u.val);
      aux = aux->next;
    }
    n = Listappend(n,aux->u.val);
  }
  return n;
}
void printList(List* l){
  List* aux = l;
  if(l==0){
    printf("[]\n");
    return ;
  }
  printf("[");
  while(aux->next!=l){
    imprimirC(aux->u.val,", ");
    aux = aux->next;
  }
  imprimirC(aux->u.val,"");
  printf("]\n");
}
