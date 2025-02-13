#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include "mlcontext.h"

//Definizione contesto mainloop
struct _mlcontext{
  pthread_t threadML;
  sigset_t signalMask;
  pthread_mutex_t mutexmlc;
  struct _EQueue *eventQueue;
};

//Creazione contesto
void mlContextNew(struct _mlcontext **mlc){
  *mlc=(struct _mlcontext *)calloc(1,sizeof(struct _mlcontext));
  EQueueNew(&( (*mlc)->eventQueue ));
  pthread_mutex_init(&( (*mlc)->mutexmlc),NULL);
  (*mlc)->threadML=pthread_self();
  
  sigset_t tmp;
  sigemptyset (&tmp);
  pthread_sigmask(SIG_BLOCK,NULL,&tmp);
  sigaddset (&tmp, SIGCONT);
  pthread_sigmask(SIG_SETMASK,&tmp,NULL);
  (*mlc)->signalMask=tmp;
}

//Eliminazione contesto
void mlContextDelete(struct _mlcontext *mlc){
  pthread_mutex_lock(&(mlc->mutexmlc));
  pthread_mutex_t tmp=mlc->mutexmlc;
  EQueueDelete(mlc->eventQueue);
  free(mlc);
  pthread_mutex_unlock(&tmp);
  pthread_mutex_destroy(&tmp);
}

//Interfaccia per segnalare il mainloop
void signalMainLoop(struct _mlcontext *mlc, uint8_t c, void* data){
  pthread_mutex_lock(&(mlc->mutexmlc));
  struct _EQueueElement *e;
  EQueueElementNew(&e,pthread_self(),c,data,NULL);
  EQueuePush(mlc->eventQueue,e);
  pthread_kill(mlc->threadML,SIGCONT);
  pthread_mutex_unlock(&(mlc->mutexmlc));
}

void signalMainLoopReply(struct _mlcontext *mlc, uint8_t c, void* data, void* res){
  if(res != NULL){
    //Imposto la maschera per la risposta
    sigset_t sigMask;
    sigemptyset (&sigMask);
    sigaddset (&sigMask, SIGCONT);
    pthread_sigmask(SIG_BLOCK,&sigMask,NULL);
    
    pthread_mutex_lock(&(mlc->mutexmlc));
    struct _EQueueElement *e;
    EQueueElementNew(&e,pthread_self(),c,data,res);
    EQueuePush(mlc->eventQueue,e);
    pthread_kill(mlc->threadML,SIGCONT);
    pthread_mutex_unlock(&(mlc->mutexmlc));
    
    //Aspetto
    int recivedSig;
    pthread_sigmask(SIG_BLOCK,NULL,&sigMask);
    sigwait(&sigMask,&recivedSig);
  }
}

//Prende il numero di notifiche in coda
int GetNSignals(struct _mlcontext *mlc){
  return GetEQueueElements(mlc->eventQueue);
}

//Estrae una notifica dalla coda
struct _EQueueElement* PopEventQueue(struct _mlcontext *mlc){
  struct _EQueueElement* e=NULL;
  EQueuePop(mlc->eventQueue, &e);
  return e;
}

void waitNewEvent(struct _mlcontext *mlc){
  if(pthread_self() == mlc->threadML){
    int recivedSig;
    sigwait(&(mlc->signalMask),&recivedSig);
  }
}

//Stampa della coda delle segnalazioni
void PrintSignalQueue(struct _mlcontext *mlc){
  PrintQueue(mlc->eventQueue);
}
