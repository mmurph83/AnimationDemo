//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2017
// Optimized C++
//----------------------------------------------------------------------------- 

#ifndef HEAP_H
#define HEAP_H

#include "Mem.h"

// Forward declaration
class Block;


// Heap class
class Heap 
{
public:
	// Constants 
	static const unsigned int NameLength = 32;
	static const unsigned int NameNumChar = (NameLength - 1);

	// Heap Types
	enum Type
	{
		Normal,
		Fixed,
		Dword = 0x7fffffff
	};

	// Info structure
	struct Info
	{
		char Name[Heap::NameLength];
		unsigned int StartAddr;
		unsigned int EndAddr;
		unsigned int TotalSize;
		int PeakNumAlloc;
		int CurrNumAlloc;
		int PeakBytesUsed;
		int CurrBytesUsed;
	};

public:
	// Public Interface (For Customers)
	Heap(unsigned int size, const char* name, Mem* mem, HANDLE heapHandle);
	void GetInfo(Heap::Info &info);
	void FreeBlock(void* p);
	// Tracking block 
	Block *GetTrackingHead();

	// Needed for Testings (Not for Customers)
	Heap *DebugGetNext() const;
	Heap *DebugGetPrev() const;
	Block* CreateBlock(size_t inSize, size_t alignSize, char *inName, int lineNum, int allocIndex);
	Heap* pNextHeap;
	Heap* pPrevHeap;
	HANDLE  mWinHeapHandle;
private:
	void AddBlock(Block* pBlock);
	// data -----------------------------------------------------------------------
	Block	*pBlkHead;
	Type	mType;
	//HANDLE  mWinHeapHandle;
	Info	mInfo;
	// Back link to the memory system
	Mem     *pMem;

};

#endif

// ---  End of File ---------------
