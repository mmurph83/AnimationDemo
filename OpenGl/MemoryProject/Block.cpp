//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2017
// Optimized C++
//----------------------------------------------------------------------------- 

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "Block.h"

#define STUB_PLEASE_REPLACE(x) (x)

Block::Block(size_t size, char* name, int lineNum, int allocIndex):pNext(nullptr),pPrev(nullptr)
{
	pNextHeap = nullptr;
	pPrevHeap = nullptr;
	mAllocSize = size;
	int i = 0;
	int startPos = 0;
	char* temp = strchr(name, '\\');
	startPos = (int)temp - (int)name;
	startPos++;
	while (temp != nullptr)
	{
		temp = strchr(name + startPos, '\\');
		if (temp != nullptr)
		{
			startPos = (int)temp - (int)name;
			startPos++;
		}
	}
	while (name[i] != '\0')
	{
		mFileName[i] = *(name + startPos);
		startPos++;
		i++;
	}
	mFileName[i] = '\0';
	mLineNum = lineNum;
	mAllocIndex = allocIndex;
	pSecret = this;
}

Block *Block::GetHeapNext()
{
	return pNext;
}

Block *Block::GetHeapPrev()
{
	return pPrev;
}

Block *Block::GetGlobalNext()
{
	return pNextHeap;
}

Block *Block::GetGlobalPrev()
{
	return pPrevHeap;
}

char *Block::GetFileName()
{
	return mFileName;
}

int Block::GetLineNum()
{
	return mLineNum;
}

int Block::GetAllocIndex()
{
	return mAllocIndex;
}

unsigned int Block::GetAllocSize()
{
	return mAllocSize;
}

// ---  End of File ---------------
