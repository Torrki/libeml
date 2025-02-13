#include <stdint.h>
#include "equeue.h"

struct _mlcontext;

void mlContextNew(struct _mlcontext **mlc);
void mlContextDelete(struct _mlcontext *mlc);
void signalMainLoop(struct _mlcontext *mlc, uint8_t c, void* data);
void signalMainLoopReply(struct _mlcontext *mlc, uint8_t c, void* data, void* res);
void waitNewEvent(struct _mlcontext *mlc);
int GetNSignals(struct _mlcontext *mlc);
struct _EQueueElement* PopEventQueue(struct _mlcontext *mlc);
void PrintSignalQueue(struct _mlcontext *mlc);
