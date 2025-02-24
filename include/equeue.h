#include <stdint.h>
#include <pthread.h>

struct _EQueueElement;
struct _EQueue;

int EQueueNew(struct _EQueue** q);
int EQueueDelete(struct _EQueue* q);
int EQueuePush(struct _EQueue* q, struct _EQueueElement* e);
int EQueuePop(struct _EQueue* q, struct _EQueueElement** e);
int EQueueElementNew(struct _EQueueElement** e, pthread_t id, uint8_t c, void* data, void* res);
int GetEQueueElements(struct _EQueue* q);
pthread_t GetIdThreadElement(struct _EQueueElement* e);
uint8_t GetCommandElement(struct _EQueueElement* e);
void* GetResAddrElement(struct _EQueueElement* e);
void* GetDataAddrElement(struct _EQueueElement* e);
void PrintQueue(struct _EQueue* q);
