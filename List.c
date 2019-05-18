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
int getListsize(List *l){
  List* aux = 0;
  int i=0;
  if(l==0)
    return 0;
  aux = l;
  while(aux->next!=l){
    i++;
    aux = aux->next;
  }
  return i+1;
}
List* getListrange(List *l,int* pos1,int* pos2){
  List* start = 0;
  List* end = 0;
  List* aux =0;
  if(l==0)
    return 0;
  if(pos1 == 0){
    start = l;
  }else{
    start = getListElement(l,*pos1);
    if(start == 0){
      if(*pos1<0)
        start = l;
      else
        return 0;
    }
  }
  if(pos2 == 0){
    end = l;
  }else{
    if(*pos2 == 0)
      return 0;
    end = getListElement(l,*pos2);
    if(end == 0){
      if(*pos2>0)
        end = l;
      else
        return 0;
    }
  }
  if(start==end)
    return 0;
  while(start->next!=end){
    aux = Listappend(aux,start->u.val);
    start = start->next;
  }
  if(pos2!=0 && *pos2!=0)
    aux = Listappend(aux,start->u.val);
  else
    if(pos2==0)
      aux = Listappend(aux,start->u.val);
  return aux;
}
List* getListElement(List *l,int pos){
  List* aux = 0;
  if(l==0)
    return 0;
  int i;
  if(pos>=0){ // Recorrido a la derecha
    aux = l;
    for(i=0;(aux->next!=l)&&(i<pos);i++){
      aux = aux->next;
    }
    if(pos>i)
      return 0;
  }
  else{ //Recorrido a la izquierda
    aux = l->prev;
    for(i=-1;(aux->prev!=l)&&(i>pos);i--){
      aux = aux->prev;
    }
    if(pos<i-1)
      return 0;
    else if(pos!=i)
      return l;
  }
  return aux;
}
ComplejoAP* getElement(List *l,int pos){
  List* aux = 0;
  if(l==0)
    return 0;
  int i;
  if(pos>=0){ // Recorrido a la derecha
    aux = l;
    for(i=0;(aux->next!=l)&&(i<pos);i++){
      aux = aux->next;
    }
    if(pos>i)
      return 0;
  }
  else{ //Recorrido a la izquierda
    aux = l->prev;
    for(i=-1;(aux->prev!=l)&&(i>pos);i--){
      aux = aux->prev;
    }
    if(pos<i-1)
      return 0;
    else if(pos!=i)
      return &(l->u.val);
  }
  return &(aux->u.val);
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
