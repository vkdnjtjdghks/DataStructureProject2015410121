#ifndef RBT_H
#define RBT_H

#include <stdio.h>
#include <stdlib.h>

typedef struct _Node {
	struct _Node *Parent;
	struct _Node *pLeft;
	struct _Node *pRight;

	enum { Red, Black } Color;

	int iID;
	void *RBData;
} Node;

void RBReleseTree(Node *pTree);

Node* RBInitNode(int iID, void *RBData);

void RBReleaseNode(Node *pNode);

Node* RBFindNode(Node *pTree, int iTargetID);

Node* RBMinNode(Node *pTree);

void RBInsertNode(Node **pTree, Node *Nil);

void RBT_InsertNodeHelper(Node **pTree, Node *Nil);

Node* RBDeleteNode(Node **pRoot, int iID);

void RBT_InserFixUp(Node **pTree, Node *Nil);

void RBT_DeleteFixUp(Node **pRoot, Node *pSuccessor);


void RBPrint(Node *pNode, int iDepth, int iBlackCount);

void RBRotateL(Node **Root, Node *Parent);

void RBRotateR(Node **pRoot, Node *Parent);

void RBTraverse(Node *pNode, void *LData, void(*fnConduct)(Node *pNode, void *LData));


#endif
