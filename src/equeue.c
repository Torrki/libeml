#include <stdio.h>
#include <stdlib.h>
#include "equeue.h"
#include "eventerr.h"

//Definizione dell'elemento
struct _EQueueElement{
  pthread_t idThread;
  uint8_t command;
  void *res;
  void* data;
  struct _EQueueElement *next;
};

//Definizione della coda
struct _EQueue{
  uint16_t nElements;
  struct _EQueueElement *head, *tail;
};

//Funzione per creare nuove code
int EQueueNew(struct _EQueue** q){
  if(q == NULL) return NULL_POINTER_ERR;
  *q=(struct _EQueue*)calloc(1,sizeof(struct _EQueue));
  return *q == NULL ? MEMORY_ERR : 0;
}

//Funzione per eliminare le code
int EQueueDelete(struct _EQueue* q){
  if(q == NULL) return NULL_POINTER_ERR;
  
  struct _EQueueElement *tmp=q->head, *next=NULL;
  while(tmp){
    next=tmp->next;
    free(tmp);
    tmp=next;
  }
  free(q);
  return 0;
}

//Aggiungere elementi alla coda
int EQueuePush(struct _EQueue* q, struct _EQueueElement* e){
  if(q == NULL || e == NULL) return NULL_POINTER_ERR;
  e->next=NULL;
  if(q->nElements==0){
    q->head=e;
  }else{
    q->tail->next=e;
  }
  q->tail=e;
  q->nElements++;
  return 0;
}

//Rimuovere elementi dalla coda
int EQueuePop(struct _EQueue* q, struct _EQueueElement** e){
  if(q == NULL || e == NULL) return NULL_POINTER_ERR;
  
  if(q->nElements >= 1){
    *e=q->head;
    if(q->nElements==1){
      q->head=NULL;
      q->tail=NULL;
    }else{
      q->head=q->head->next;
    }
    q->nElements--;
  }else{
    *e=NULL;
  }
  return 0;
}

//Crea un nuovo elemento per la coda
int EQueueElementNew(struct _EQueueElement** e, pthread_t id, uint8_t c, void* data, void* res){
  if(e == NULL) return NULL_POINTER_ERR;
  
  *e=(struct _EQueueElement*)calloc(1,sizeof(struct _EQueueElement));
  if(*e == NULL) return MEMORY_ERR;
  
  (*e)->idThread=id;
  (*e)->command=c;
  (*e)->res=res;
  (*e)->data=data;
  return 0;
}

//Ottiene il numero di elementi della coda
int GetEQueueElements(struct _EQueue* q){
  return q==NULL ? -1 : q->nElements;
}

//Ottiene l'ID del thread
pthread_t GetIdThreadElement(struct _EQueueElement* e){
  return e->idThread;
}

//Ottiene il comando dell'elemento
uint8_t GetCommandElement(struct _EQueueElement* e){
  return e->command;
}

void* GetResAddrElement(struct _EQueueElement* e){
  return e->res;
}

void* GetDataAddrElement(struct _EQueueElement* e){
  return e->data;
}

void PrintQueue(struct _EQueue* q){
  struct _EQueueElement *tmp=q->head;
  while(tmp){
    printf("(%lu,%hhu)-->",tmp->idThread,tmp->command);
    tmp=tmp->next;
  }
  printf("null\n");
}
