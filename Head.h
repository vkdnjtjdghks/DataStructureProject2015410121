#ifndef FnHelper_h
#define FnHelper_h
#define EDGE 9999999
#include <stdio.h>
#include "LinkedList.h"
#include "RBTree.h"


typedef struct _User {
	char szName[100];
	int iFollowNum;
	LinkedList *pFollow;
	int iTweetNum;
	LinkedList *pTweet;
} User;
typedef struct _WordNum {
	char szWord[1000];
	int iCnt;
} WordNum;

typedef struct _Tuple {
	void *pFirst;
	void *pSecond;
} Tuple;

int fnTweetUserComp(void *pLeft, void *pRight);
void GetUserWhoTweetWord(Node *pNode, void *pData);
int fnTweetWordComp(void *pLeft, void *pRight);

void CountUserNum(Node *pNode, void *pData);

void CountFollowNum(Node *pNode, void *pData);
void MinFollowNum(Node *pNode, void *pData);
void MaxFollowNum(Node *pNode, void *pData);

void CountTweetNum(Node *pNode, void *pData);
void MinTweetNum(Node *pNode, void *pData);

void MaxTweetNum(Node *pNode, void *pData);

void PrintUserWhoTweetWord(Node *pNode, void *pData);

void GetUserIDFromName(Node *pNode, void *pData);

void DeleteWord(Node *pNode, void *pData);

void DeleteUserFromWord(Node *pNode, void *pData);

void DeleteFollowFromList(Node *pNode, void *pData);

void EnqueueNode(Node *pNode, void *pData);

void SearchWordNum(Node *pNode, void *pData);


#endif
