#include <stdint.h>
#include <pthread.h>

struct _EQueueElement;
struct _EQueue;

void EQueueNew(struct _EQueue** q);
void EQueueDelete(struct _EQueue* q);
void EQueuePush(struct _EQueue* q, struct _EQueueElement* e);
void EQueuePop(struct _EQueue* q, struct _EQueueElement** e);
void EQueueElementNew(struct _EQueueElement** e, pthread_t id, uint8_t c, void* data, void* res);
int GetEQueueElements(struct _EQueue* q);
pthread_t GetIdThreadElement(struct _EQueueElement* e);
uint8_t GetCommandElement(struct _EQueueElement* e);
void* GetResAddrElement(struct _EQueueElement* e);
void* GetDataAddrElement(struct _EQueueElement* e);
void PrintQueue(struct _EQueue* q);
