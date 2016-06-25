#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "LinkedList.h"
#include "RBTree.h"
#include "Head.h"
#include "PQueue.h"
#include "Hash.h"

Node *null;

int main() {
	Node *pTree = NULL;
	int choice = 0;	
	null = RBInitNode(0, 0);
	null->Color = Node::Black;
	do {
		//메뉴 디스플레이
		int N;
		printf("0. Read data files\n");
		printf("1. Display statistics\n");
		printf("2. Top 5 most tweeted words\n");
		printf("3. Top 5 most tweeted users\n");
		printf("4. Find users who tweeted a word\n");
		printf("5. Find all people who are friends of the above users\n");
		printf("6. Delete all mentions of a word\n");
		printf("7. Delete all users who mentioned a word\n");
		printf("8. Find strongly connected components\n");
		printf("9. Find shortest path from a given user\n");
		printf("99. Quit\n");
		printf("Select Menu:   ");
		scanf("%d", &N);
		choice = N;
		switch (choice) {
		case 0:
		{
			int NumofUser = 0;
			int NumofTweet = 0;
			int NumofFriends = 0;

			char temp[1000];
			//파일을 읽는다
			FILE *fp = fopen("user.txt", "r");


			while (fgets(temp, sizeof(temp), fp) != NULL) {

				if (!(temp[0] == '\n' || temp[0] == ' ' || temp[0] == 't')) {
					char *pos;//계행처리
					int ID = atoi(temp);
					fgets(temp, sizeof(temp), fp);//날짜무시
					fgets(temp, sizeof(temp), fp);
		if ((pos = strchr(temp, '\n')) != NULL)
						*pos = '\0';


					NumofUser = NumofUser + 1;

					User* pUser = (User*)malloc(sizeof(User));//유저 정보
					pUser->iFollowNum = 0;
					pUser->pFollow = NULL;
					pUser->iTweetNum = 0;
					pUser->pTweet = NULL;

					strcpy(pUser->szName, temp);// 이름복사

					RBInsertNode(&pTree, RBInitNode(ID, pUser));//레드블랙트리에 ID와 유저정보를 넣는다
				}
			}
			fclose(fp);

			fp = fopen("friend.txt", "r");

			while (fgets(temp, sizeof(temp), fp) != NULL) {

				if (!(temp[0] == '\n' || temp[0] == ' ' ||temp[0] =='t')) {
					int ID = atoi(temp);
					Node *pUserNode = RBFindNode(pTree, ID);//노드 서치 
					User *pUser = (User*)pUserNode->RBData;
					fgets(temp, sizeof(temp), fp);
					int FriendID = atoi(temp);
					int* pFollow = (int*)malloc(sizeof(int));
					*pFollow = FriendID;
					NumofFriends++;
					LN_InsertNode(&pUser->pFollow, pFollow);//링크드리스트 사용 친구구현
					pUser->iFollowNum++;
				}
			}
			fclose(fp);
			fp = fopen("word.txt", "r");
			while (fgets(temp, sizeof(temp), fp) != NULL) {

				if (!(temp[0] == '\n'|| temp[0] == ' '||temp[0]=='t')) {
					int ID = atoi(temp);
					Node *pUserNode = RBFindNode(pTree, ID);
					User *pUser = (User*)pUserNode->RBData;
					fgets(temp, sizeof(temp), fp);
					fgets(temp, sizeof(temp), fp);
					char *pos;
					if ((pos = strchr(temp, '\n')) != NULL)
						*pos = '\0';
					NumofTweet = NumofTweet + 1;
					char* pTweet = (char*)malloc(sizeof(char) * strlen(temp));
					strcpy(pTweet, temp);


					LN_InsertNode(&pUser->pTweet, pTweet);
					pUser->iTweetNum++;
				}
			}

			fclose(fp);
			printf("\nTotal users : %d\n", NumofUser);
			printf("Total friendship records : %d\n", NumofFriends);
			printf("Total tweets : %d\n\n", NumofTweet);

		}
		break;
		case 1:
		{
			int NumofUsers = NULL;
			int NumofFriends = NULL;
			int iTweetNum = NULL;		
			int iMinFollowNum = EDGE;
			int iMaxFollowNum = NULL;
			int iMinTweetNum = EDGE;
			int iMaxTweetNum = NULL;
			RBTraverse(pTree, &NumofUsers, CountUserNum);
			RBTraverse(pTree, &NumofFriends, CountFollowNum);
			RBTraverse(pTree, &iTweetNum, CountTweetNum);
			RBTraverse(pTree, &iMinFollowNum, MinFollowNum);
			RBTraverse(pTree, &iMaxFollowNum, MaxFollowNum);
			RBTraverse(pTree, &iMinTweetNum, MinTweetNum);
			RBTraverse(pTree, &iMaxTweetNum, MaxTweetNum);
			//순회
			printf("\nAverage number of friends : %d\n", NumofFriends / NumofUsers);
			printf("Minimum friends : %d\n", iMinFollowNum);
			printf("Maximum friends : %d\n", iMaxFollowNum);
			printf("Average tweets per user : %d\n", iTweetNum / NumofUsers);
			printf("Minimum tweets per user : %d\n", iMinTweetNum);
			printf("Maximum tweets per user : %d\n", iMaxTweetNum);
		}
		break;
		case 2:
		{
			int iTweetNum = 0;
			RBTraverse(pTree, &iTweetNum, CountTweetNum);

			HashTable* pHash = CreateHashTable(iTweetNum);

			RBTraverse(pTree, pHash, SearchWordNum);// 순회후

			PQueue pq;//우선순위 큐

			PQueueInit(&pq, iTweetNum, fnTweetWordComp);

			for (int i = 0; i < iTweetNum; i++) {
				int iWordNum = pHash->Table[i].HInt;
				if (iWordNum > 0) {
					WordNum *pWordNum = (WordNum*)malloc(sizeof(WordNum));
					strcpy(pWordNum->szWord, pHash->Table[i].HData);
					pWordNum->iCnt = iWordNum;
					PEnqueue(&pq, pWordNum);
				}
			}

			printf("\nTop 5 most tweeted words : \n\n");
			for (int i = 0; i < 5; i++) {

				WordNum* pWordNum = (WordNum*)PDequeue(&pq);
				printf("%s : %d\n", pWordNum->szWord, pWordNum->iCnt);

				free(pWordNum);
			}
			puts;

			while (!PQIsEmpty(&pq)) {
				WordNum* pWordNum = (WordNum*)PDequeue(&pq);
				free(pWordNum);
			}

			PQueueDestroy(&pq);
		}
		break;
		case 3:
		{
			int iUserNum = 0;
			RBTraverse(pTree, &iUserNum, CountUserNum);

			PQueue pq;

			PQueueInit(&pq, iUserNum, fnTweetUserComp);

			RBTraverse(pTree, &pq, EnqueueNode);//우선순위큐

			printf("\n");
			printf("Top 5 most tweeted users\n\n");
			for (int i = 0; i < 5; i++) {

				Node* pNode = (Node*)PDequeue(&pq);
				User* pUser = (User*)pNode->RBData;
				printf("[%s] : %d\n", pUser->szName, pUser->iTweetNum);
			}
			printf("\n");


		}


		
		break;

		case 4:
			char temp[1000];

			printf("Type tweet word : ");
			scanf("%s", temp);


			printf("\n");
			printf("Users : ");
			RBTraverse(pTree, temp, PrintUserWhoTweetWord);//순회
			printf("\n\n");
			break;

		case 5:
		{

			char buf[512];
			int iUserNum = 0;
			RBTraverse(pTree, &iUserNum, CountUserNum);

			int* pUserList = (int*)malloc(sizeof(int) * iUserNum);
			memset(pUserList, 0, sizeof(int) * iUserNum);
			Tuple tuple = { buf,pUserList };

			printf("Input tweet : ");//4번반복
			scanf("%s", buf);

			PQueue pq;

			PQueueInit(&pq, iUserNum, fnTweetUserComp);

			RBTraverse(pTree, &pq, EnqueueNode);

			printf("\n");
			printf("Top 5 most tweeted users\n\n");

		
			for (int i = 0; i < 5; i++) {

				Node* pNode = (Node*)PDequeue(&pq);
				User* pUser = (User*)pNode->RBData;
				printf("Top 5 Most tweeted - %d [%s] : %d Follows\n", i + 1, pUser->szName, pUser->iFollowNum);

				LinkedList *pCurrentNode = pUser->pFollow;

				printf("Follows : ");
				while (pCurrentNode != NULL) {
					int FriendID = *(int*)pCurrentNode->LData;

					Node *pFollowNode = RBFindNode(pTree, FriendID);
					User *pFollowUser = (User*)pFollowNode->RBData;

					printf("%s ", pFollowUser->szName);

					pCurrentNode = pCurrentNode->next;
				}
				printf("\n\n");

			}
			printf("\n");




			RBTraverse(pTree, &tuple, GetUserWhoTweetWord);

			int Cnt = 0;
			while (*(pUserList + Cnt) != 0) {
				Node *pUserNode = RBFindNode(pTree, *(pUserList + Cnt));
				User* pUser = (User*)pUserNode->RBData;

				printf("tweet \'%s\' - [%s] : %d Follows\n", buf, pUser->szName, pUser->iFollowNum);

				LinkedList *pCurrentNode = pUser->pFollow;

				printf("Follows : ");
				while (pCurrentNode != NULL) {
					int FriendID = *(int*)pCurrentNode->LData;

					Node *pFollowNode = RBFindNode(pTree, FriendID);
					User *pFollowUser = (User*)pFollowNode->RBData;

					printf("%s ", pFollowUser->szName);

					pCurrentNode = pCurrentNode->next;
				}
				printf("\n\n");

				Cnt++;
			}


			free(pUserList);


		}
		break;
		
		case 6:
		{
			
			char temp[512];

			printf("\n");
			printf("Type tweet word : ");
			scanf("%s", temp);

			RBTraverse(pTree, temp, DeleteWord);//순회후 단어가 같으면 삭제

		//힙 메모리 에러가 떠서 결국 고치지 못함.. 는 고침
		}
		break;

		case 7:
		{
		
			char temp[1000];

			printf("\n");
			printf("Type tweet word : ");
			scanf("%s", temp);
			printf("\n");

			int iUserNum = 0;
			RBTraverse(pTree, &iUserNum, CountUserNum);

			int* pDeleteUserList = (int*)malloc(sizeof(int) * iUserNum);
			memset(pDeleteUserList, 0, sizeof(int) * iUserNum);
			Tuple tuple = { temp,pDeleteUserList };

			RBTraverse(pTree, &tuple, DeleteUserFromWord);

			int Cnt = 0;
			while (*(pDeleteUserList + Cnt) != 0) {
				
				Node *pDeleteNode = RBDeleteNode(&pTree, *(pDeleteUserList + Cnt));

				RBReleaseNode(pDeleteNode);

				Cnt++;
			}//데이터구조에서 유저삭제

			RBTraverse(pTree, pDeleteUserList, DeleteFollowFromList);

			free(pDeleteUserList);
		
			//이것도 결국 고치지 못함... 는 고침
		}
		break;
		case 8:
		{
		
		}
		break;
		case 9:
		{
			
		}
		break;
		default:
			break;
	}
}while (choice != 99); RBReleseTree(pTree);
return 0;
	} 

	

	

