#ifndef PQueue_h
#define PQueue_h

#include <stdio.h>
#include "Heap.h"

typedef Heap PQueue;

void PQueueInit(PQueue * ppq, int size, PriorityComp pc);
int PQIsEmpty(PQueue * ppq);

void PEnqueue(PQueue * ppq, void* pData);
void* PDequeue(PQueue * ppq);
void PQueueDestroy(PQueue * ppq);

#endif


