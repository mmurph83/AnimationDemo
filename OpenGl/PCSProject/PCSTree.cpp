//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2017
// Optimized C++
//----------------------------------------------------------------------------- 

#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "PCSTree.h"
#include "PCSNode.h"

#define STUB_PLEASE_REPLACE(x) (x)

// constructor
PCSTree::PCSTree() : pRoot(nullptr)
{
	STUB_PLEASE_REPLACE(0);
	mInfo = Info{ 0,0,0,0 };
};

// destructor
PCSTree::~PCSTree()
{
	STUB_PLEASE_REPLACE(0);
};


// get Root
PCSNode *PCSTree::GetRoot() const
{
	return pRoot;
}


// insert
void PCSTree::Insert(PCSNode * const pInNode, PCSNode * const pParent)
{
	assert(pInNode != nullptr);
	mInfo.currNumNodes++;
	if (mInfo.currNumNodes > mInfo.maxNumNodes)
	{
		mInfo.maxNumNodes = mInfo.currNumNodes;
	}
	if (pRoot == nullptr)
	{
		pRoot = pInNode;
		pRoot->SetParent(nullptr);
		pInNode->SetForward(nullptr);
		pInNode->SetReverse(nullptr);
	}
	else
	{
		assert(pParent != nullptr);
		pInNode->SetParent(pParent);
		PCSNode* pTempChild = pParent->GetChild();
		if (pTempChild != nullptr)
		{
			pInNode->SetNextSibling(pTempChild);
			pInNode->SetPrevSibling(nullptr);
			pTempChild->SetPrevSibling(pInNode);
			
		}
		pParent->SetChild(pInNode);
		pInNode->SetForward(pParent->GetForward());
		pParent->SetForward(pInNode);
		PCSNode* pReverseTemp = FindPrevReverseNode(pInNode);
		if (pReverseTemp != nullptr)
		{
			pReverseTemp->SetReverse(pInNode);
		}
		pInNode->SetReverse(FindNextReverseNode(pInNode));
	}
}
PCSNode* PCSTree::FindNextReverseNode(PCSNode* pNode)
{
	PCSNode* pTemp = pNode->GetNextSibling();
	while (pTemp != nullptr)
	{
		if (pTemp->GetChild() == nullptr)
		{
			return pTemp;
		}
		pTemp = pTemp->GetChild();
	}
	if (pTemp == nullptr)
	{
		pTemp = pNode->GetParent();
	}
	return pTemp;
}

PCSNode* PCSTree::FindPrevReverseNode(PCSNode* pNode)
{
	PCSNode* pTemp = pNode;
	while (pTemp != nullptr)
	{
		if (pTemp->GetPrevSibling())
		{
			return pTemp->GetPrevSibling();
		}
		pTemp = pTemp->GetParent();
	}
	return pTemp;
}
PCSNode* PCSTree::FindLastForwardNode(PCSNode* pNode)
{
	PCSNode* pTemp = pNode->GetChild();
	while (pTemp != nullptr)
	{
		if (pTemp->GetNextSibling())
		{
			pTemp = pTemp->GetNextSibling();
		}
		else if (pTemp->GetChild())
		{
			pTemp = pTemp->GetChild();
		}
		else
		{
			return pTemp;
		}
	}
	return pNode;
}
// Remove
void PCSTree::Remove(PCSNode * const pInNode)
{
	//What if I remove the root or the parent?
	STUB_PLEASE_REPLACE(pInNode);
	if (pInNode != pRoot)
	{
		PCSNode* pTemp = FindPrevReverseNode(pInNode);
		if (pTemp != nullptr)
		{
			pTemp->SetReverse(pInNode->GetReverse());
		}
		
		if (pInNode->GetPrevSibling())
		{
			pTemp = FindLastForwardNode(pInNode->GetPrevSibling());
			pTemp->SetForward(FindLastForwardNode(pInNode)->GetForward());
			//pTemp->SetForward(pInNode->GetForward());
		}
		else
		{
			pTemp = FindLastForwardNode(pInNode);
			pInNode->GetParent()->SetForward(pTemp->GetForward());
		}
		RemoveChildren(pInNode);
		PCSNode* pPrevSibling = pInNode->GetPrevSibling();
		PCSNode* pNextSibling = pInNode->GetNextSibling();
		PCSNode* pParent = pInNode->GetParent();
		mInfo.currNumNodes--;
		if (pPrevSibling == nullptr)
		{
			pParent->SetChild(pNextSibling);
		}
		else
		{
			pPrevSibling->SetNextSibling(pNextSibling);
		}
		if (pNextSibling != nullptr)
		{
			pNextSibling->SetPrevSibling(pPrevSibling);
		}
		pInNode->SetParent(nullptr);
		pInNode->SetNextSibling(nullptr);
		pInNode->SetPrevSibling(nullptr);
		pInNode->SetForward(nullptr);
		pInNode->SetReverse(nullptr);
	}
	else
	{
		DismantleTree();
	}
}

// get tree info
void PCSTree::GetInfo(Info &info)
{
	CheckDepth();
	info.currNumLevels = mInfo.currNumLevels;
	info.currNumNodes = mInfo.currNumNodes;
	info.maxNumLevels = mInfo.maxNumLevels;
	info.maxNumNodes = mInfo.maxNumNodes;
}

void PCSTree::CheckDepth()
{

	PCSNode* pTemp = pRoot;
	int currentDepth = 0;
	int maxDepth = 0;
	if (pTemp != nullptr)
	{
		currentDepth++;
		while (pTemp != nullptr)
		{
			if (pTemp->GetChild() != nullptr)
			{
				pTemp = pTemp->GetChild();
				currentDepth++;
			}
			else if (pTemp->GetNextSibling() != nullptr)
			{
				pTemp = pTemp->GetNextSibling();
			}
			else
			{
				if (currentDepth > maxDepth)
				{
					maxDepth = currentDepth;
				}
				while (pTemp != nullptr)
				{
					if (pTemp->GetNextSibling() != nullptr)
					{
						pTemp = pTemp->GetNextSibling();
						break;
					}
					else
					{
						currentDepth--;

						pTemp = pTemp->GetParent();
					}
				}
			}
		}
	}
	if (mInfo.maxNumLevels < maxDepth)
	{
		mInfo.maxNumLevels = maxDepth;
	}
	mInfo.currNumLevels = maxDepth;
}
void PCSTree::DismantleTree()
{
	PCSNode *pTemp = pRoot;
	while (pRoot->GetNumChildren() != 0)
	{
		while (pTemp->GetChild() != 0)
		{
			pTemp = pTemp->GetChild();
		}
		pTemp = pTemp->GetParent();
		Remove(pTemp->GetChild());
	}
	mInfo.currNumLevels = 0;
	mInfo.currNumNodes = 0;
	pRoot = nullptr;
}

void PCSTree::RemoveChildren(PCSNode* pNode)
{
	PCSNode* pTemp = pNode->GetChild();
	PCSNode* pTempSibling = nullptr;
	if (pTemp != nullptr)
	{
		while (pTemp != pNode)
		{
			while (pTemp->GetChild() != nullptr)
			{
				pTemp = pTemp->GetChild();
			}
			mInfo.currNumNodes--;
			pTempSibling = pTemp->GetNextSibling();
			if (pTempSibling != nullptr)
			{
				pTempSibling->SetPrevSibling(nullptr);
				pTempSibling->GetParent()->SetChild(pTempSibling);
				pTemp->SetNextSibling(nullptr);
				pTemp->SetParent(nullptr);
				pTemp->SetForward(nullptr);
				pTemp->SetReverse(nullptr);
				pTemp = pTempSibling;
			}
			else
			{
				pTemp->SetForward(nullptr);
				pTemp->SetReverse(nullptr);
				pTemp = pTemp->GetParent();
				pTemp->GetChild()->SetParent(nullptr);
				pTemp->SetChild(nullptr);
			}
		}
	}
}

void PCSTree::Print() const
{
	printf("Printing tree as Child, Sibling, Parent order\n");
	PCSNode* pTemp = GetRoot();
	while (pTemp != nullptr)
	{
		while (pTemp->GetChild() != nullptr)
		{
			pTemp = pTemp->GetChild();
		}
		pTemp->PrintNode();
		if (pTemp->GetNextSibling() != nullptr)
		{
			pTemp = pTemp->GetNextSibling();
		}
		else
		{
			pTemp = pTemp->GetParent();
			while (pTemp != nullptr)
			{
				pTemp->PrintNode();
				if (pTemp->GetNextSibling() != nullptr)
				{
					pTemp = pTemp->GetNextSibling();
					break;
				}
				else
				{
					pTemp = pTemp->GetParent();
				}
			}
		}
	}
	printf("\n");
}


// ---  End of File ---------------
