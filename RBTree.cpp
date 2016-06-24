#include "RBTree.h"

extern Node *null;

void RBReleseTree(Node *pTree) {
	if (pTree == NULL)
		return;

	if (pTree->pRight != null)
		RBReleseTree(pTree->pRight);

	if (pTree->pLeft != null)
		RBReleseTree(pTree->pLeft);

	pTree->pLeft = null;
	pTree->pRight = null;

	RBReleaseNode(pTree);
}

Node* RBInitNode(int iID, void *RBData) {
	Node *Nil = (Node*)malloc(sizeof(Node));
	Nil->Parent = NULL;
	Nil->pLeft = NULL;
	Nil->pRight = NULL;
	Nil->iID = iID;
	Nil->RBData = RBData;
	Nil->Color = _Node::Black;

	return Nil;
}


void RBReleaseNode(Node *pNode) {
	free(pNode);
}

Node* RBFindNode(Node *pTree, int iTargetID) {
	if (pTree == null)
		return NULL;

	
	if (pTree->iID > iTargetID)
		return RBFindNode(pTree->pLeft, iTargetID);
	else if (pTree->iID < iTargetID)
		return RBFindNode(pTree->pRight, iTargetID);
	else
		return pTree;

}

Node* RBMinNode(Node *pTree) {
	if (pTree == null)
		return null;

	if (pTree->pLeft == null)
		return pTree;
	else
		return RBMinNode(pTree->pLeft);
}

void RBInsertNode(Node **pTree, Node *Nil) {
	RBT_InsertNodeHelper(pTree, Nil);

	Nil->Color = _Node::Red;
	Nil->pLeft = null;
	Nil->pRight = null;

	RBT_InserFixUp(pTree, Nil);
}

void RBT_InsertNodeHelper(Node **pTree, Node *Nil) {
	if ((*pTree) == NULL)
		(*pTree) = Nil;

	
	if ((*pTree)->iID < Nil->iID) {
		if ((*pTree)->pRight == null) {
			(*pTree)->pRight = Nil;
			Nil->Parent = (*pTree);
		}
		else
			RBT_InsertNodeHelper(&(*pTree)->pRight, Nil);
	}
	else if ((*pTree)->iID > Nil->iID) {
		if ((*pTree)->pLeft == null) {
			(*pTree)->pLeft = Nil;
			Nil->Parent = (*pTree);
		}
		else
			RBT_InsertNodeHelper(&(*pTree)->pLeft, Nil);
	}
}

Node* RBDeleteNode(Node **pRoot, int iID) {
	Node *pRemoved = NULL;
	Node *pSuccessor = NULL;
	Node *pTarget = RBFindNode((*pRoot), iID);

	if (pTarget == NULL)
		return NULL;

	if (pTarget->pLeft == null || pTarget->pRight == null)
		pRemoved = pTarget;
	else {
		pRemoved = RBMinNode(pTarget->pRight);
		pTarget->RBData = pRemoved->RBData;
		pTarget->iID = pRemoved->iID;
	}

	if (pRemoved->pLeft != null)
		pSuccessor = pRemoved->pLeft;
	else
		pSuccessor = pRemoved->pRight;

	pSuccessor->Parent = pRemoved->Parent;

	if (pRemoved->Parent == NULL) {
		(*pRoot) = pSuccessor;
	}
	else {
		if (pRemoved == pRemoved->Parent->pLeft)
			pRemoved->Parent->pLeft = pSuccessor;
		else
			pRemoved->Parent->pRight = pSuccessor;
	}

	if (pRemoved->Color == _Node::Black)
		RBT_DeleteFixUp(pRoot, pSuccessor);


	return pRemoved;
}

void RBT_InserFixUp(Node **pTree, Node *Nil) {
	while (Nil != (*pTree) && Nil->Parent->Color == _Node::Red) {
		if (Nil->Parent == Nil->Parent->Parent->pLeft) {
			Node* pUncle = Nil->Parent->Parent->pRight;

			if (pUncle->Color == _Node::Red) {
				Nil->Parent->Color = _Node::Black;
				pUncle->Color = _Node::Black;
				Nil->Parent->Parent->Color = _Node::Red;

				Nil = Nil->Parent->Parent;
			}
			else {
				if (Nil == Nil->Parent->pRight) {
					Nil = Nil->Parent;
					RBRotateL(pTree, Nil);
				}

				Nil->Parent->Color = _Node::Black;
				Nil->Parent->Parent->Color = _Node::Red;

				RBRotateR(pTree, Nil->Parent->Parent);
			}
		}
		else {
			Node *pUncle = Nil->Parent->Parent->pLeft;

			if (pUncle->Color == _Node::Red) {
				Nil->Parent->Color = _Node::Black;
				pUncle->Color = _Node::Black;
				Nil->Parent->Parent->Color = _Node::Red;

				Nil = Nil->Parent->Parent;
			}
			else {
				if (Nil == Nil->Parent->pLeft) {
					Nil = Nil->Parent;
					RBRotateR(pTree, Nil);
				}

				Nil->Parent->Color = _Node::Black;
				Nil->Parent->Parent->Color = _Node::Red;
				RBRotateL(pTree, Nil->Parent->Parent);
			}
		}
	}

	(*pTree)->Color = _Node::Black;
}

void RBT_DeleteFixUp(Node **pRoot, Node *pSuccessor) {
	Node *pSibling = NULL;

	while (pSuccessor->Parent != NULL && pSuccessor->Color == _Node::Black) {
		if (pSuccessor == pSuccessor->Parent->pLeft) {
			pSibling = pSuccessor->Parent->pRight;

			if (pSibling->Color == _Node::Red) {
				pSibling->Color = _Node::Black;
				pSuccessor->Parent->Color = _Node::Red;
				RBRotateL(pRoot, pSuccessor->Parent);
			
			}
			else {
				if (pSibling->pLeft->Color == _Node::Black && pSibling->pRight->Color == _Node::Black) {
					pSibling->Color = _Node::Red;
					pSuccessor = pSuccessor->Parent;
				}
				else {
					if (pSibling->pLeft->Color == _Node::Red) {
						pSibling->pLeft->Color = _Node::Black;
						pSibling->Color = _Node::Red;

						RBRotateR(pRoot, pSibling);
						pSibling = pSuccessor->Parent->pRight;
					}

					pSibling->Color = pSuccessor->Parent->Color;
					pSuccessor->Parent->Color = _Node::Black;
					pSibling->pRight->Color = _Node::Black;
					RBRotateL(pRoot, pSuccessor->Parent);
					pSuccessor = (*pRoot);
				}
			}
		}
		else {
			pSibling = pSuccessor->Parent->pLeft;

			if (pSibling->Color == _Node::Red) {
				pSibling->Color = _Node::Black;
				pSuccessor->Parent->Color = _Node::Red;
				RBRotateR(pRoot, pSuccessor->Parent);
				
			}
			else {
				if (pSibling->pRight->Color == _Node::Black && pSibling->pLeft->Color == _Node::Black) {
					pSibling->Color = _Node::Red;
					pSuccessor = pSuccessor->Parent;
				}
				else {
					if (pSibling->pRight->Color == _Node::Red) {
						pSibling->pRight->Color = _Node::Black;
						pSibling->Color = _Node::Red;

						RBRotateL(pRoot, pSibling);
						pSibling = pSuccessor->Parent->pLeft;
					}

					pSibling->Color = pSuccessor->Parent->Color;
					pSuccessor->Parent->Color = _Node::Black;
					pSibling->pLeft->Color = _Node::Black;
					RBRotateR(pRoot, pSuccessor->Parent);
					pSuccessor = (*pRoot);
				}
			}
		}
	}

	pSuccessor->Color = _Node::Black;
}




void RBRotateL(Node **pRoot, Node *Parent) {
	Node *pRightChild = Parent->pRight;

	Parent->pRight = pRightChild->pLeft;

	if (pRightChild->pLeft != null)
		pRightChild->pLeft->Parent = Parent;

	pRightChild->Parent = Parent->Parent;

	if (Parent->Parent == NULL)
		(*pRoot) = pRightChild;
	else {
		if (Parent == Parent->Parent->pLeft)
			Parent->Parent->pLeft = pRightChild;
		else
			Parent->Parent->pRight = pRightChild;
	}
	pRightChild->pLeft = Parent;
	Parent->Parent = pRightChild;
}

void RBRotateR(Node **pRoot, Node *Parent) {
	Node *pLeftChild = Parent->pLeft;

	Parent->pLeft = pLeftChild->pRight;

	if (pLeftChild->pRight != null)
		pLeftChild->pRight->Parent = Parent;

	pLeftChild->Parent = Parent->Parent;

	if (Parent->Parent == NULL)
		(*pRoot) = pLeftChild;
	else {
		if (Parent == Parent->Parent->pLeft)
			Parent->Parent->pLeft = pLeftChild;
		else
			Parent->Parent->pRight = pLeftChild;
	}
	pLeftChild->pRight = Parent;
	Parent->Parent = pLeftChild;
}



void RBTraverse(Node *pNode, void *LData, void(*fnConduct)(Node *pNode, void *LData)) {
	if (pNode == NULL || pNode == null)
		return;

	RBTraverse(pNode->pLeft, LData, fnConduct);

	fnConduct(pNode, LData);

	RBTraverse(pNode->pRight, LData, fnConduct);
}

void RBPrint(Node *pNode, int iDepth, int iBlackCount) {
	int i = 0;
	char c = 'X';
	int v = -1;
	char cnt[100];

	if (pNode == NULL || pNode == null)
		return;

	if (pNode->Color == _Node::Black)
		iBlackCount++;

	if (pNode->Parent != NULL) {
		v = pNode->Parent->iID;

		if (pNode->Parent->pLeft == pNode)
			c = 'L';
		else
			c = 'R';
	}

	if (pNode->pLeft == null && pNode->pRight == null)
		sprintf(cnt, "--------- %d", iBlackCount);
	else
		sprintf(cnt, "");

	for (i = 0; i<iDepth; i++)
		printf("   ");

	printf("%d %s [%c, %d] %s\n", pNode->iID,
		(pNode->Color == _Node::Red) ? "Red" : "Black", c, v, cnt);

	RBPrint(pNode->pLeft, iDepth + 1, iBlackCount);
	RBPrint(pNode->pRight, iDepth + 1, iBlackCount);

}

