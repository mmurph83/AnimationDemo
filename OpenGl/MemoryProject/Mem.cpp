//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2017
// Optimized C++
//----------------------------------------------------------------------------- 

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "GlobalNew.h"

#include "Mem.h"
#include "Heap.h"
#include "Block.h"

#define STUB_PLEASE_REPLACE(x) (x)

// Create the heap.
Mem::Code Mem::HeapCreate(Heap *&newHeap, unsigned int heapSize, const char * const Name)
{
	if (Instance()->mInitialized)
	{
		heapSize += sizeof(Heap);
		void * temp = ::HeapCreate(HEAP_GENERATE_EXCEPTIONS, heapSize, 0);
		printf("%x\n", (unsigned int)temp);
		newHeap = (Heap*)HeapAlloc(temp, HEAP_ZERO_MEMORY, sizeof(Heap));
		newHeap = new (newHeap) Heap(heapSize, Name, Instance(), temp);
		if (Instance()->poHead == nullptr)
		{
			Instance()->poHead = newHeap;
		}
		else
		{
			newHeap->pNextHeap = Instance()->poHead;
			Instance()->poHead->pPrevHeap = newHeap;
			Instance()->poHead = newHeap;
		}
		//newHeap = (Heap*)new Heap();
		Instance()->mInfo.CurrHeapCount++;
		if (Instance()->mInfo.CurrHeapCount > Instance()->mInfo.PeakHeapCount)
		{
			Instance()->mInfo.PeakHeapCount = Instance()->mInfo.CurrHeapCount;
		}
		if (newHeap != nullptr)
		{
			return Mem::Code::OK;
		}
	}
	return Code::ERROR_Mem_Not_Initialized;
}
Mem * Mem::Instance()
{
	static Mem mem;
	return &mem;
}

Mem::Code Mem::HeapCreate(Heap *&newHeap, unsigned int numBlocks, int sizePerBlock, const char * const Name)
{
	STUB_PLEASE_REPLACE(newHeap);
	STUB_PLEASE_REPLACE(numBlocks);
	STUB_PLEASE_REPLACE(sizePerBlock);
	STUB_PLEASE_REPLACE(Name);
	
	return (Mem::Code)STUB_PLEASE_REPLACE(0);
}

// Initialize the memory system  (Public method)
Mem::Code Mem::Initialize(void)
{
	if (!Instance()->mInitialized)
	{
		Instance()->mInitialized = true;
		Instance()->mInfo.CurrAllocIndex = 0;
		Instance()->mInfo.CurrBytesUsed = 0;
		Instance()->mInfo.CurrHeapCount = 0;
		Instance()->mInfo.CurrNumAlloc = 0;
		Instance()->mInfo.PeakBytesUsed = 0;
		Instance()->mInfo.PeakHeapCount = 0;
		Instance()->mInfo.PeakNumAlloc = 0;
		Instance()->pGlobalHead = nullptr;
		return Mem::Code::OK;
	}
	else
	{
		Mem::Destroy();
		return Mem::Initialize();
	}
	
}

Mem::Code Mem::Destroy(void)
{
	STUB_PLEASE_REPLACE(0);
	if(Instance()->mInitialized)
	{
		while (Instance()->poHead != nullptr)
		{
			HeapDestroy(Instance()->poHead);
		}
		Instance()->mInitialized = false;
		return Mem::OK;
	}
	return Mem::ERROR_Mem_Not_Initialized;
}

Mem::Code Mem::GetInfo(Mem::Info &retInfo)
{
	if (Instance()->mInitialized)
	{
		retInfo = Instance()->mInfo;


		Heap* pTemp = Instance()->poHead;
		if (pTemp != nullptr)
		{
			retInfo.CurrNumAlloc = 0;
			retInfo.CurrBytesUsed = 0;
			Heap::Info tempRetInfo;
			while (pTemp != nullptr)
			{
				pTemp->GetInfo(tempRetInfo);
				retInfo.CurrNumAlloc += tempRetInfo.CurrNumAlloc;
				if (retInfo.CurrNumAlloc > retInfo.PeakNumAlloc)
				{
					retInfo.PeakNumAlloc = retInfo.CurrNumAlloc;
				}
				retInfo.CurrBytesUsed += tempRetInfo.CurrBytesUsed;
				if (retInfo.CurrBytesUsed > retInfo.PeakBytesUsed)
				{
					retInfo.PeakBytesUsed = retInfo.CurrBytesUsed;
				}
				pTemp = pTemp->DebugGetNext();
			}
			if (Instance()->mInfo.PeakNumAlloc < retInfo.PeakNumAlloc)
			{
				Instance()->mInfo.PeakNumAlloc = retInfo.PeakNumAlloc;
				Instance()->mInfo.CurrAllocIndex = retInfo.PeakNumAlloc;
			}
			if (Instance()->mInfo.PeakBytesUsed < retInfo.PeakBytesUsed)
			{
				Instance()->mInfo.PeakBytesUsed = retInfo.PeakBytesUsed;
			}
			retInfo.PeakNumAlloc = Instance()->mInfo.PeakNumAlloc;
			retInfo.PeakHeapCount = Instance()->mInfo.PeakHeapCount;
			retInfo.PeakBytesUsed = Instance()->mInfo.PeakBytesUsed;
			retInfo.CurrAllocIndex = Instance()->mInfo.PeakNumAlloc;
		}
		else
		{
			Instance()->mInfo.CurrBytesUsed = 0;
			Instance()->mInfo.CurrHeapCount = 0;
			Instance()->mInfo.CurrNumAlloc = 0;
			retInfo = Instance()->mInfo;
		}
		return Mem::OK;
	}
	return Mem::ERROR_Mem_Not_Initialized;
}
void Mem::AttachBlock(Block* pBlock)
{
	if (pGlobalHead == nullptr)
	{
		pGlobalHead = pBlock;
		pGlobalHead->pNextHeap = nullptr;
		pGlobalHead->pPrevHeap = nullptr;
	}
	else
	{
		pBlock->pNextHeap = pGlobalHead;
		pGlobalHead->pPrevHeap = pBlock;
		pBlock->pPrevHeap = nullptr;
		pGlobalHead = pBlock;
	}
}
void* Mem::CreateBlock(size_t inSize, Heap *pHeap, Mem::Align align, char *inName, int lineNum)
{
	STUB_PLEASE_REPLACE(inSize);
	STUB_PLEASE_REPLACE(pHeap);
	STUB_PLEASE_REPLACE(align);
	STUB_PLEASE_REPLACE(inName);
	STUB_PLEASE_REPLACE(lineNum);
	Instance()->mInfo.CurrBytesUsed += inSize;
	if (Instance()->mInfo.CurrBytesUsed > Instance()->mInfo.PeakBytesUsed)
	{
		Instance()->mInfo.PeakBytesUsed = Instance()->mInfo.CurrBytesUsed;
	}
	Instance()->mInfo.CurrNumAlloc++;
	if (Instance()->mInfo.CurrNumAlloc > Instance()->mInfo.PeakNumAlloc)
	{
		Instance()->mInfo.PeakNumAlloc = Instance()->mInfo.CurrNumAlloc;
		Instance()->mInfo.CurrAllocIndex = Instance()->mInfo.CurrNumAlloc;
	}
	unsigned int number = 1;
	switch (align)
	{
	case Mem::Align::Byte_4:
		number = 4;
		break;
	case Mem::Align::Byte_8:
		number = 8;
		break;
	case Mem::Align::Byte_16:
		number = 16;
		break;
	case Mem::Align::Byte_32:
		number = 32;
		break;
	case Mem::Align::Byte_64:
		number = 64;
		break;
	case Mem::Align::Byte_128:
		number = 128;
		break;
	case Mem::Align::Byte_256:
		number = 256;
		break;
	default:
		number = 256;
		break;
	}
	//HeapCreate(HEAP_GENERATE_EXCEPTIONS)
	return pHeap->CreateBlock(inSize, number, inName,lineNum, Instance()->mInfo.CurrAllocIndex);
}
void Mem::FreeBlock(void * p)
{
	Heap* temp = Instance()->poHead;
	unsigned int addressOffset;
	Heap::Info info;
	while (temp != nullptr)
	{
		temp->GetInfo(info);
		
		addressOffset = (unsigned int)p - (unsigned int)temp;
		if ((unsigned int)p > (unsigned int)temp && (unsigned int)p < (unsigned int)temp + info.TotalSize)
		{
			temp->FreeBlock(p);
			return;
		}
		temp = temp->DebugGetNext();
	}
}
void Mem::RemoveBlock(Block* pBlock)
{
	if (pBlock->pPrevHeap == nullptr)
	{
		pGlobalHead = pGlobalHead->pNextHeap;
		if (pGlobalHead != nullptr)
		{
			pGlobalHead->pPrevHeap = nullptr;
		}
	}
	else
	{
		pBlock->pPrevHeap->pNextHeap = pBlock->pNextHeap;
	}
	if (pBlock->pNextHeap != nullptr)
	{
		pBlock->pNextHeap->pPrevHeap = pBlock->pPrevHeap;
	}
}
Heap *Mem::DebugGetHeapHead()
{
	return Instance()->poHead;
}


Block *Mem::DebugGetGlobalTrackingHead()
{
	return Instance()->pGlobalHead;
}

Mem::Code Mem::HeapDestroy(Heap *inHeap)
{
	if (Instance()->mInitialized)
	{
		if (inHeap != nullptr)
		{
			if (inHeap->pNextHeap != nullptr)
			{
				inHeap->pNextHeap->pPrevHeap = inHeap->pPrevHeap;
			}
			if (inHeap->pPrevHeap != nullptr)
			{
				inHeap->pPrevHeap->pNextHeap = inHeap->pNextHeap;
			}
			else
			{
				Instance()->poHead = Instance()->poHead->pNextHeap;
			}
			Instance()->mInfo.CurrHeapCount--;
			::HeapDestroy(inHeap->mWinHeapHandle);
			return Code::OK;
		}
	}
	return Code::ERROR_Mem_Not_Initialized;
}


Mem::Code Mem::GetHeapByAddr(Heap *&pHeap, void *p)
{
	STUB_PLEASE_REPLACE(pHeap);
	STUB_PLEASE_REPLACE(p);
	return Code::ERROR_Mem_Not_Initialized;
}

// ---  End of File ---------------
