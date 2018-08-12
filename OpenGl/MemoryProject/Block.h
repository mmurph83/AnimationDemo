//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2017
// Optimized C++
//----------------------------------------------------------------------------- 

#ifndef BLOCK_H
#define BLOCK_H

#include "Heap.h"

class Block
{
public:
	// defines for the heap name length
	static const unsigned int NameLength = 32;
	static const unsigned int NameNumChar = (Heap::NameLength - 1);

public:
	// Public methods
	Block(size_t size, char* name, int lineNum, int allocIndex);
	Block *GetHeapNext();
	Block *GetHeapPrev();
	Block *GetGlobalNext();
	Block *GetGlobalPrev();
	char *GetFileName();
	int GetLineNum();
	int GetAllocIndex();
	unsigned int GetAllocSize();
	
private:
	// Total Size of this class:
	// 64 Bytes - add appropriate padding

	// Note: need to add Tracking Block links for Heap and Global lists
	//       Several ways to do it... I don't care
	//       No Templates

	// Total Size of this class:
	// 64 Bytes - add appropriate padding

   // Required
	char           mFileName[Block::NameLength];
	int            mLineNum;
	int            mAllocIndex;
	unsigned int   mAllocSize;
public:
	Block* pNext;
	Block* pPrev;
	Block* pNextHeap;
	Block* pPrevHeap;
public:
	Block          *pSecret;   // back pointer to the top of the trackingblock
};

#endif

// ---  End of File ---------------
