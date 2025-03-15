#include <stdint.h>
#include "equeue.h"

typedef struct _mlcontext MlContext;

int mlContextNew(MlContext **mlc);
int mlContextDelete(MlContext *mlc);
int signalMainLoop(MlContext *mlc, uint8_t c, void* data);
int signalMainLoopReply(MlContext *mlc, uint8_t c, void* data, void* res);
int waitNewEvent(MlContext *mlc);
int GetNSignals(MlContext *mlc);
EQueueElement* PopEventQueue(MlContext *mlc);
void PrintSignalQueue(MlContext *mlc);
