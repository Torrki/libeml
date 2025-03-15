#include <stdint.h>
#include <pthread.h>

typedef struct _EQueueElement EQueueElement;
typedef struct _EQueue EQueue;

int EQueueNew(EQueue** q);
int EQueueDelete(EQueue* q);
int EQueuePush(EQueue* q, EQueueElement* e);
int EQueuePop(EQueue* q, EQueueElement** e);
int EQueueElementNew(EQueueElement** e, pthread_t id, uint8_t c, void* data, void* res);
int GetEQueueElements(EQueue* q);
pthread_t GetIdThreadElement(EQueueElement* e);
uint8_t GetCommandElement(EQueueElement* e);
void* GetResAddrElement(EQueueElement* e);
void* GetDataAddrElement(EQueueElement* e);
void PrintQueue(EQueue* q);
