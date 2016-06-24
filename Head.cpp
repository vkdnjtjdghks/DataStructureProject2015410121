#include "Head.h"
#include "PQueue.h"
#include "Hash.h"
#include <string.h>

int fnTweetUserComp(void *pLeft, void *pRight) {
	Node *pLeftNode = (Node*)pLeft;
	Node *pRightNode = (Node*)pRight;

	User *pLeftUser = (User*)pLeftNode->RBData;
	User *pRightUser = (User*)pRightNode->RBData;
	return pLeftUser->iTweetNum - pRightUser->iTweetNum;
}

int fnTweetWordComp(void *pLeft, void *pRight) {
	WordNum *pLeftWordNum = (WordNum*)pLeft;
	WordNum *pRightWordNum = (WordNum*)pRight;

	return pLeftWordNum->iCnt - pRightWordNum->iCnt;

}

void CountUserNum(Node *pNode, void *pData) {
	int* pNum = (int*)pData;
	(*pNum) += 1;
}

void CountFollowNum(Node *pNode, void *pData) {
	int* pNum = (int*)pData;
	User* pUser = (User*)pNode->RBData;
	(*pNum) += pUser->iFollowNum;
}

void MinFollowNum(Node *pNode, void *pData) {
	int* pNum = (int*)pData;
	User* pUser = (User*)pNode->RBData;
	if ((*pNum) > pUser->iFollowNum)
		(*pNum) = pUser->iFollowNum;
}

void MaxFollowNum(Node *pNode, void *pData) {
	int* pNum = (int*)pData;
	User* pUser = (User*)pNode->RBData;
	if ((*pNum) < pUser->iFollowNum)
		(*pNum) = pUser->iFollowNum;
}


void CountTweetNum(Node *pNode, void *pData) {
	int* pNum = (int*)pData;
	User* pUser = (User*)pNode->RBData;
	(*pNum) += pUser->iTweetNum;
}

void MinTweetNum(Node *pNode, void *pData) {
	int* pNum = (int*)pData;
	User* pUser = (User*)pNode->RBData;
	if ((*pNum) > pUser->iTweetNum)
		(*pNum) = pUser->iTweetNum;
}

void MaxTweetNum(Node *pNode, void *pData) {
	int* pNum = (int*)pData;
	User* pUser = (User*)pNode->RBData;
	if ((*pNum) < pUser->iTweetNum)
		(*pNum) = pUser->iTweetNum;
}

void PrintUserWhoTweetWord(Node *pNode, void *pData) {
	char *pWord = (char*)pData;
	User* pUser = (User*)pNode->RBData;

	LinkedList *pCurrentNode = pUser->pTweet;

	while (pCurrentNode != NULL) {

		char *pUserWord = (char*)pCurrentNode->LData;

		if (strcmp(pWord, pUserWord) == 0) {
			printf("%s ", pUser->szName);
			break;
		}

		pCurrentNode = pCurrentNode->next;
	}

}
void GetUserWhoTweetWord(Node *pNode, void *pData) {
	Tuple *pTuple = (Tuple*)pData;
	char* pWord = (char*)pTuple->pFirst;
	int* pUserList = (int*)pTuple->pSecond;
	User* pUser = (User*)pNode->RBData;

	LinkedList *pCurrentNode = pUser->pTweet;

	
	while (pCurrentNode != NULL) {

		char *pUserWord = (char*)pCurrentNode->LData;

		if (strcmp(pWord, pUserWord) == 0) {

			int Cnt = 0;
			while (*(pUserList + Cnt) != 0)
				Cnt++;

			*(pUserList + Cnt) = pNode->iID;

			break;
		}

		pCurrentNode = pCurrentNode->next;
	}
}




void GetUserIDFromName(Node *pNode, void *pData) {
	Tuple *pTuple = (Tuple*)pData;
	char* pName = (char*)pTuple->pFirst;
	int* pUserID = (int*)pTuple->pSecond;
	User* pUser = (User*)pNode->RBData;

	if (strcmp(pName, pUser->szName) == 0) {
		(*pUserID) = pNode->iID;
	}
}

void DeleteWord(Node *pNode, void *pData) {
	char *pWord = (char*)pData;
	User* pUser = (User*)pNode->RBData;

	LinkedList *pCurrentNode = pUser->pTweet;

	while (pCurrentNode != NULL) {
		LinkedList *pNextNode = pCurrentNode->next;
		char* pUserWord = (char*)pCurrentNode->LData;


		if (strcmp(pWord, pUserWord) == 0) {
		
			LN_DeleteNode(&pUser->pTweet, pCurrentNode);
			pUser->iTweetNum--;
		}

		pCurrentNode = pNextNode;
	}
}


void DeleteUserFromWord(Node *pNode, void *pData) {
	Tuple *pTuple = (Tuple*)pData;
	char* pWord = (char*)pTuple->pFirst;
	int* pUserList = (int*)pTuple->pSecond;
	User* pUser = (User*)pNode->RBData;

	LinkedList *pCurrentNode = pUser->pTweet;

	while (pCurrentNode != NULL) {
		char* pUserWord = (char*)pCurrentNode->LData;

		if (strcmp(pWord, pUserWord) == 0) {

			while (pUser->pTweet != NULL) {
				char* pUserWord = (char*)(pUser->pTweet->LData);

			
				LN_DeleteNode(&pUser->pTweet, pUser->pTweet);
				pUser->iTweetNum--;
			}
			while (pUser->pFollow != NULL) {
				int* pUserID = (int*)(pUser->pFollow->LData);

			
				LN_DeleteNode(&pUser->pFollow, pUser->pFollow);
				pUser->iFollowNum--;
			}

			int Cnt = 0;
			while (*(pUserList + Cnt) != 0)
				Cnt++;

			*(pUserList + Cnt) = pNode->iID;


			return;
		}

		pCurrentNode = pCurrentNode->next;
	}
}

void DeleteFollowFromList(Node *pNode, void *pData) {
	int* pUserList = (int*)pData;
	User* pUser = (User*)pNode->RBData;

	LinkedList *pCurrentNode = pUser->pFollow;

	while (pCurrentNode != NULL) {
		LinkedList *pNextNode = pCurrentNode->next;

		int* pFollowID = (int*)pCurrentNode->LData;
		for (int i = 0; pUserList[i] != 0; i++) {
			if (pUserList[i] == *pFollowID) {
				free(pFollowID);
				LN_DeleteNode(&pUser->pFollow, pCurrentNode);
				pUser->iFollowNum--;
			}
		}

		pCurrentNode = pNextNode;
	}
}

void EnqueueNode(Node *pNode, void *pData) {
	PQueue* queue = (PQueue*)pData;

	PEnqueue(queue, pNode);
}

void SearchWordNum(Node *pNode, void *pData) {
	User* pUser = (User*)pNode->RBData;
	HashTable* pHash = (HashTable*)pData;

	LinkedList *pCurrentNode = pUser->pTweet;

	while (pCurrentNode != NULL) {
		char* pUserWord = (char*)pCurrentNode->LData;

		Hash_Set(&pHash, pUserWord, Hash_Get(pHash, pUserWord) + 1);

		pCurrentNode = pCurrentNode->next;
	}

}
