//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2017
// Optimized C++
//----------------------------------------------------------------------------- 

#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "PCSTree.h"
#include "PCSNode.h"

#define STUB_PLEASE_REPLACE(x) (x)

// constructor
PCSNode::PCSNode() : pParent(nullptr), pChild(nullptr), pNextSibling(nullptr), pPrevSibling(nullptr), pForward(nullptr),pReverse(nullptr)
{
	memset(&pName[0], 0x0, PCSNode::NAME_SIZE);
}

// copy constructor
PCSNode::PCSNode(const PCSNode &in) : pParent(in.pParent), pChild(in.pChild), pNextSibling(in.pNextSibling), pPrevSibling(in.pPrevSibling), pForward(in.pForward),pReverse(in.pReverse)
{
	SetName(in.pName);
}

// Specialize Constructor
PCSNode::PCSNode(PCSNode * const pInParent, PCSNode * const pInChild, PCSNode * const pInNextSibling, PCSNode * const pInPrevSibling, const char * const pInName)
	: pParent(pInParent), pChild(pInChild), pNextSibling(pInNextSibling), pPrevSibling(pInPrevSibling), pForward(nullptr), pReverse(nullptr)
{
	SetName(pInName);
}

PCSNode::PCSNode(const char * const pInName) : pParent(nullptr), pChild(nullptr), pNextSibling(nullptr), pPrevSibling(nullptr), pForward(nullptr), pReverse(nullptr)
{
	assert(pInName != nullptr);
	for (int i = 0; i < PCSNode::NAME_SIZE; i++)
	{
		pName[i] = pInName[i];
	}
}

// destructor
PCSNode::~PCSNode()
{
	STUB_PLEASE_REPLACE(0);
}

// assignment operator
PCSNode &PCSNode::operator = (const PCSNode &in)
{
	STUB_PLEASE_REPLACE(in);
	pParent = in.pParent;
	pChild = in.pChild;
	pNextSibling = in.pNextSibling;
	pPrevSibling = in.pPrevSibling;
	for (int i = 0; i < PCSNode::NAME_SIZE; i++)
	{
		pName[i] = in.pName[i];
	}
	return *this;
}

void PCSNode::SetParent(PCSNode * const pIn)
{
	pParent = pIn;
}

void PCSNode::SetChild(PCSNode * const pIn)
{
	pChild = pIn;

}

void PCSNode::SetNextSibling(PCSNode * const pIn)
{
	pNextSibling = pIn;
}

void PCSNode::SetPrevSibling(PCSNode * const pIn)
{
	pPrevSibling = pIn;
}

PCSNode *PCSNode::GetParent(void) const
{
	return pParent;
}

PCSNode *PCSNode::GetChild(void) const
{
	return pChild;
}

PCSNode *PCSNode::GetNextSibling(void) const
{
	return pNextSibling;
}

PCSNode *PCSNode::GetPrevSibling(void) const
{
	return pPrevSibling;
}

PCSNode::Code PCSNode::SetName(const char * const pInName)
{
	if (pInName == nullptr)
	{
		return PCSNode::Code::FAIL_NULL_PTR;
	}
	for (int i = 0; i < PCSNode::NAME_SIZE; i++)
	{
		pName[i] = pInName[i];
	}
	return PCSNode::Code::SUCCESS;
}

PCSNode::Code PCSNode::GetName(char * const pOutBuffer, unsigned int sizeOutBuffer) const
{
	if (pOutBuffer == nullptr)
	{
		return PCSNode::Code::FAIL_NULL_PTR;
	}
	for (unsigned int i = 0; i < sizeOutBuffer && i < NAME_SIZE; i++)
	{
		pOutBuffer[i] = pName[i];
	}
	if (sizeOutBuffer < NAME_SIZE)
	{
		pOutBuffer[sizeOutBuffer - 1] = '\0';
	}
	else
	{
		pOutBuffer[NAME_SIZE - 1] = '\0';
	}
	return PCSNode::Code::SUCCESS;
}

void PCSNode::PrintNode() const
{
	for (int i = 0; pName[i] != '\0'; i++)
	{
		printf("%c", pName[i]);
	}
	printf(" ");
}

void PCSNode::PrintChildren() const
{
	PCSNode* pTemp = GetChild();
	while (pTemp != nullptr)
	{
		pTemp->PrintNode();
		pTemp = pTemp->GetNextSibling();
	}
}


void PCSNode::PrintSiblings() const
{
	if (GetParent() != nullptr)
	{
		PCSNode* pTemp = GetParent()->GetChild();
		while (pTemp != nullptr)
		{
			if (pTemp != this)
			{
				pTemp->PrintNode();
			}
			pTemp = pTemp->GetNextSibling();
		}
	}
}

PCSNode *PCSNode::GetForward(void) const
{
	return pForward;
}

PCSNode *PCSNode::GetReverse(void) const
{
	return pReverse;
}

void PCSNode::SetForward(PCSNode * const pIn)
{
	pForward = pIn;
}
void PCSNode::SetReverse(PCSNode * const pIn)
{
	pReverse = pIn;
}

void PCSNode::privSetForward()
{
	pForward = GetParent()->GetForward();
	GetParent()->SetForward(this);
}

int PCSNode::GetNumSiblings() const
{
	if (GetParent() != nullptr)
	{
		int returnVal = 0;
		PCSNode* pTemp = GetParent()->GetChild();
		while (pTemp != nullptr)
		{
			returnVal++;
			pTemp = pTemp->GetNextSibling();
		}
		return returnVal;
	}
	return 1;
}

int PCSNode::GetNumChildren() const
{
	int returnVal = 0;
	PCSNode* pTemp = GetChild();
	while (pTemp != nullptr)
	{
		returnVal++;
		pTemp = pTemp->GetNextSibling();
	}
	return returnVal;
}