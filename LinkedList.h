#ifndef LinkedList_h
#define LinkedList_h

#include <stdio.h>
#include <stdlib.h>


typedef struct _LinkedList {
	void* LData;
	struct _LinkedList* next;
} LinkedList;

void LN_InsertNode(LinkedList **pHead, void* LData);

void LN_DeleteNode(LinkedList **pHead, LinkedList* pNode);


#endif
