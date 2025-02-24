#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include "mlcontext.h"
#include "eventerr.h"

//Definizione contesto mainloop
struct _mlcontext{
  pthread_t threadML;
  sigset_t signalMask;
  pthread_mutex_t mutexmlc;
  struct _EQueue *eventQueue;
};

//Creazione contesto
int mlContextNew(struct _mlcontext **mlc){
  if(mlc == NULL) return NULL_POINTER_ERR;
  
  *mlc=(struct _mlcontext *)calloc(1,sizeof(struct _mlcontext));
  if(*mlc == NULL) return MEMORY_ERR;
  
  int ret=EQueueNew(&( (*mlc)->eventQueue ));
  if(ret != 0) return ret;
  
  ret=pthread_mutex_init(&( (*mlc)->mutexmlc),NULL);
  if(ret != 0) return MUTEX_INIT_ERR;
  
  (*mlc)->threadML=pthread_self();
  
  sigset_t tmp;
  ret=0;
  ret |= sigemptyset(&tmp);
  ret |= pthread_sigmask(SIG_BLOCK,NULL,&tmp);
  ret |= sigaddset (&tmp, SIGCONT);
  ret |= pthread_sigmask(SIG_SETMASK,&tmp,NULL);
  
  if(ret != 0) return SIGMASK_ERR;
  (*mlc)->signalMask=tmp;
  return 0;
}

//Eliminazione contesto
int mlContextDelete(struct _mlcontext *mlc){
  if(mlc == NULL) return NULL_POINTER_ERR;
  int ret=0;
  pthread_mutex_t tmp=mlc->mutexmlc;
  
  if(pthread_mutex_lock(&(mlc->mutexmlc)) != 0) return MUTEX_LOCK_ERR;
  ret=EQueueDelete(mlc->eventQueue);
  if(ret != 0) return ret;
  
  free(mlc);
  
  ret=pthread_mutex_unlock(&tmp);
  if(ret != 0) return MUTEX_UNLOCK_ERR;
  ret=pthread_mutex_destroy(&tmp);
  if(ret != 0) return MUTEX_DESTROY_ERR;
  return 0;
}

//Interfaccia per segnalare il mainloop
int signalMainLoop(struct _mlcontext *mlc, uint8_t c, void* data){
  struct _EQueueElement *e;
  int ret=0;
  if(mlc == NULL) return NULL_POINTER_ERR;
  
  if(pthread_mutex_lock(&(mlc->mutexmlc)) != 0) return MUTEX_LOCK_ERR;
  ret=EQueueElementNew(&e,pthread_self(),c,data,NULL);
  if(ret != 0) return ret;
  ret=EQueuePush(mlc->eventQueue,e);
  if(ret != 0) return ret;
  ret=pthread_kill(mlc->threadML,SIGCONT);
  if(ret != 0) return THREAD_KILL_ERR;
  ret=pthread_mutex_unlock(&(mlc->mutexmlc));
  if(ret != 0) return MUTEX_UNLOCK_ERR;
  return 0;
}

int signalMainLoopReply(struct _mlcontext *mlc, uint8_t c, void* data, void* res){
  if(mlc == NULL || res == NULL) return NULL_POINTER_ERR;
  
  //Imposto la maschera per la risposta
  int ret=0;
  sigset_t sigMask;
  struct _EQueueElement *e;
  ret |= sigemptyset (&sigMask);
  ret |= sigaddset (&sigMask, SIGCONT);
  ret |= pthread_sigmask(SIG_BLOCK,&sigMask,NULL);
  if(ret != 0) return SIGMASK_ERR;
  
  ret=pthread_mutex_lock(&(mlc->mutexmlc));
  if(ret != 0) return MUTEX_LOCK_ERR;
  ret=EQueueElementNew(&e,pthread_self(),c,data,res);
  if(ret != 0) return ret;
  ret=EQueuePush(mlc->eventQueue,e);
  if(ret != 0) return ret;
  ret=pthread_kill(mlc->threadML,SIGCONT);
  if(ret != 0) return THREAD_KILL_ERR;
  ret=pthread_mutex_unlock(&(mlc->mutexmlc));
  if(ret != 0) return MUTEX_UNLOCK_ERR;
  
  //Aspetto
  int recivedSig;
  ret=0;
  ret |= pthread_sigmask(SIG_BLOCK,NULL,&sigMask);
  ret |= sigwait(&sigMask,&recivedSig);
  return ret != 0 ? SIGMASK_ERR : 0;
}

//Prende il numero di notifiche in coda
int GetNSignals(struct _mlcontext *mlc){
  return GetEQueueElements(mlc->eventQueue);
}

//Estrae una notifica dalla coda
struct _EQueueElement* PopEventQueue(struct _mlcontext *mlc){
  struct _EQueueElement* e=NULL;
  int ret=0;
  
  ret=EQueuePop(mlc->eventQueue, &e);
  return ret == 0 ? e : NULL;
}

int waitNewEvent(struct _mlcontext *mlc){
  if(mlc == NULL) return NULL_POINTER_ERR;
  
  if(pthread_self() == mlc->threadML){
    int recivedSig;
    int ret=sigwait(&(mlc->signalMask),&recivedSig);
    return ret == 0 ? 0 : SIGMASK_ERR;
  }else{
    return THREAD_NOT_ML_ERR;
  }
}

//Stampa della coda delle segnalazioni
void PrintSignalQueue(struct _mlcontext *mlc){
  if(mlc != NULL){
    PrintQueue(mlc->eventQueue);
  }
}
