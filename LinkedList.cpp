#include "LinkedList.h"


void LN_InsertNode(LinkedList **pHead, void* LData) {
	
	LinkedList* Nil = (LinkedList*)malloc(sizeof(LinkedList));
	Nil->LData = LData;
	Nil->next = NULL;

	if (*pHead == NULL)
		*pHead = Nil;
	else {
		LinkedList *pCurrentNode = *pHead;

		while (pCurrentNode->next != NULL)
			pCurrentNode = pCurrentNode->next;

		pCurrentNode->next = Nil;
	}
};

void LN_DeleteNode(LinkedList **pHead, LinkedList* pNode) {

	if (*pHead == NULL || pNode == NULL) {
		return;
	}

	LinkedList *pCurrentNode = (*pHead);
	LinkedList* pDeleted=NULL;

	
	if (pCurrentNode == pNode) {
		pDeleted = (*pHead);

		*pHead = (*pHead)->next;
	}
	else {
		while (pCurrentNode->next != NULL) {
			if (pCurrentNode->next == pNode) {
				pDeleted = pCurrentNode->next;

				pCurrentNode->next = pCurrentNode->next->next;

				break;
			}

			pCurrentNode = pCurrentNode->next;
		}
	}

	free(pDeleted);
}
