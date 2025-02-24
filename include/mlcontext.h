#include <stdint.h>
#include "equeue.h"

struct _mlcontext;

int mlContextNew(struct _mlcontext **mlc);
int mlContextDelete(struct _mlcontext *mlc);
int signalMainLoop(struct _mlcontext *mlc, uint8_t c, void* data);
int signalMainLoopReply(struct _mlcontext *mlc, uint8_t c, void* data, void* res);
int waitNewEvent(struct _mlcontext *mlc);
int GetNSignals(struct _mlcontext *mlc);
struct _EQueueElement* PopEventQueue(struct _mlcontext *mlc);
void PrintSignalQueue(struct _mlcontext *mlc);
