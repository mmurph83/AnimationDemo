//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2017
// Optimized C++
//----------------------------------------------------------------------------- 

#include <assert.h>
#include <new.h>  // for placement new

#include "Heap.h"
#include "Block.h"

#define STUB_PLEASE_REPLACE(x) (x)

Heap::Heap(unsigned int size, const char* name, Mem* mem, HANDLE heapHandle): pNextHeap(nullptr), pPrevHeap(nullptr)
{
	pMem = mem;
	mInfo.TotalSize = size - sizeof(Heap);
	int i = 0;
	while (name[i] != '\0')
	{
		mInfo.Name[i] = name[i];
		i++;
	}
	mInfo.Name[i] = '\0';
	mInfo.StartAddr = (unsigned int)this;
	mInfo.EndAddr = size - sizeof(Heap) + (unsigned int)this;
	mInfo.CurrBytesUsed = 0;
	mInfo.PeakBytesUsed = 0;
	mInfo.PeakNumAlloc = 0;
	mInfo.CurrNumAlloc = 0;
	mWinHeapHandle = heapHandle;
	pBlkHead = nullptr;
	
}
void Heap::FreeBlock(void* p)
{
	
	int* temp = (int*)p;
	temp -= 1;
	temp = (int*)*temp;
	Block* temp2 = (Block*)temp;
	pMem->RemoveBlock(temp2);
	if (temp2->pPrev == nullptr)
	{
		pBlkHead = pBlkHead->pNext;
		if (pBlkHead != nullptr)
		{
			pBlkHead->pPrev = nullptr;
		}
	}
	else
	{
		temp2->pPrev->pNext = temp2->pNext;
	}
	if (temp2->pNext != nullptr)
	{
		temp2->pNext->pPrev = temp2->pPrev;
		temp2->pNext = nullptr;
		temp2->pPrev = nullptr;
	}
	mInfo.CurrBytesUsed -= temp2->GetAllocSize();
	mInfo.CurrNumAlloc--;
	HeapFree(mWinHeapHandle, HEAP_NO_SERIALIZE, temp);
}
Block* Heap::CreateBlock(size_t inSize, size_t alignSize, char *inName, int lineNum, int allocIndex)
{
	
	
	
	unsigned int addressOffset;
	
	Block* pBlock = (Block*)HeapAlloc(mWinHeapHandle, HEAP_GENERATE_EXCEPTIONS, sizeof(Block) + inSize);

	addressOffset = (unsigned int)(pBlock)+sizeof(Block);
	unsigned int hexNum = 0xFF;
	while (hexNum > alignSize)
	{
		hexNum >>= 1;
	}
	addressOffset &= hexNum;
	if (addressOffset != 0)
	{
		addressOffset = alignSize - addressOffset;
		pBlock = (Block*)HeapReAlloc(mWinHeapHandle, HEAP_ZERO_MEMORY, pBlock, sizeof(Block) + inSize + addressOffset);
	}
	
	//pBlock = (Block*)HeapAlloc(mWinHeapHandle, HEAP_ZERO_MEMORY, sizeof(Block) + inSize + addressOffset);
	pBlock = new(pBlock)Block(inSize, inName, lineNum,allocIndex);
	pMem->AttachBlock(pBlock);
	AddBlock(pBlock);

	mInfo.CurrBytesUsed += inSize;
	if (mInfo.PeakBytesUsed < mInfo.CurrBytesUsed)
	{
		mInfo.PeakBytesUsed = mInfo.CurrBytesUsed;
	}
	
	mInfo.CurrNumAlloc++;
	if (mInfo.CurrNumAlloc > mInfo.PeakNumAlloc)
	{
		mInfo.PeakNumAlloc = mInfo.CurrNumAlloc;
	}
	unsigned int address = (unsigned int)pBlock;
	pBlock += 1;
	pBlock = (Block *)((char*)pBlock + addressOffset);
	unsigned int* temp = (unsigned int*)pBlock;
	temp -= 1;
	*temp = address;
	//char *p = new(pBlock)char[300];

	//FreeBlock(p);
	return pBlock;
}

void Heap::AddBlock(Block* pBlock)
{
	if (pBlkHead == nullptr)
	{
		pBlkHead = pBlock;
		pBlock->pNext = nullptr;
		pBlock->pPrev = nullptr;
		
	}
	else
	{
		pBlock->pNext = pBlkHead;
		pBlkHead->pPrev = pBlock;
		pBlkHead = pBlock;
		pBlkHead->pPrev = nullptr;
	}
}

void Heap::GetInfo(Heap::Info &retInfo)
{
	retInfo = mInfo;
};

Heap *Heap::DebugGetNext() const
{
	return 	pNextHeap;
};

Heap *Heap::DebugGetPrev() const
{
	return 	pPrevHeap;
};

Block *Heap::GetTrackingHead()
{
   return pBlkHead;
}

// ---  End of File ---------------
