#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include "mlcontext.h"
#include "equeue.h"

/*
Progetto per la creazione di un mainloop per una programmazione orientata agli eventi:
  1. Serve una risorsa condivisa che permetta di inviare dei messaggi(segnali) da thread a mainloop
  2. il mainloop vede quali messaggi ci sono in coda e li processa
  3. se non ci sono messaggi in coda il thread si auto sospende
Una implementazione molto elementare in Linux è usare lo heap dato che è l'unica regione di memoria tra i thread che viene condivisa
*/

void* Thread_1(void* args);
void* Thread_2(void* args);

int main(int argc, char *argv[]){
  //Creazione del contesto
  struct _mlcontext *MLC;
  pthread_t idThreads[5];
  mlContextNew(&MLC);
  
  pthread_create(idThreads,NULL,Thread_1,MLC);
  pthread_create(idThreads+1,NULL,Thread_1,MLC);
  pthread_create(idThreads+2,NULL,Thread_1,MLC);
  pthread_create(idThreads+3,NULL,Thread_2,MLC);
  pthread_create(idThreads+4,NULL,Thread_1,MLC);
  uint8_t lastCommand=0;
  
  //Ciclo di elaborazione degli eventi, termino con 2
  while(lastCommand != 2){
    if(GetNSignals(MLC)){
      struct _EQueueElement *event=PopEventQueue(MLC);
      lastCommand=GetCommandElement(event);
      switch(lastCommand){
        case 1:
          printf("Comando 1\n");
          break;
        case 3:
          printf("Comando 3\n");
          char* resAddr=(char*)GetResAddrElement(event);
          strcpy(resAddr, "Hello Main Loop");
          pthread_kill(GetIdThreadElement(event),SIGCONT);
          break;
      }
      free(event);
    }else{
      //Punto di sospensione se non ci sono eventi
      waitNewEvent(MLC);
    }
  }
  PrintSignalQueue(MLC);
  
  mlContextDelete(MLC);
  return 0;
}

void* Thread_1(void* args){
  struct _mlcontext *MLC=(struct _mlcontext *)args;
  signalMainLoop(MLC,1,NULL);
  return NULL;
}

void* Thread_2(void* args){
  struct _mlcontext *MLC=(struct _mlcontext *)args;
  char *reply=(char*)calloc(20,sizeof(char));
  signalMainLoopReply(MLC,3,NULL,(void*)reply);
  
  printf("Risposta da ML arrivata: %s\n", reply);
  free(reply);
  signalMainLoop(MLC,2,NULL);
  return NULL;
}
