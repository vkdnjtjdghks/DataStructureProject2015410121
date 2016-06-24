#include "PQueue.h"

void PQueueInit(PQueue * ppq, int size, PriorityComp pc)
{
	HeapInit(ppq, size, pc);
}

int PQIsEmpty(PQueue * ppq)
{
	return HIsEmpty(ppq);
}
void PQueueDestroy(PQueue * ppq) {
	HeapDestroy(ppq);
}



void PEnqueue(PQueue * ppq, void* pData)
{
	HInsert(ppq, pData);
}

void* PDequeue(PQueue * ppq)
{
	return HDelete(ppq);
}


