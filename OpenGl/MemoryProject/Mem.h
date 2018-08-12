//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2017
// Optimized C++
//----------------------------------------------------------------------------- 

#ifndef MEM_H
#define MEM_H

// forward declaration
class Heap;
class Block;


// Singleton
class Mem
{
public:

	// Constants for the library
	static const unsigned int MAJOR_VERSION = 2;
	static const unsigned int MINOR_VERSION = 0;

	// alignment 
	enum Align
	{
		Byte_4,
		Byte_8,
		Byte_16,
		Byte_32,
		Byte_64,
		Byte_128,
		Byte_256
	};

	// Return codes
	enum Code
	{
		OK,
		ERROR_Heap_Create,
		ERROR_Illegal_Heap,
		ERROR_Mem_Not_Initialized,
		ERROR_Invalid_Addr,
		Dword = 0x7fffffff
	};

	// info structure
	struct Info
	{
		int CurrAllocIndex;
		int PeakHeapCount;
		int CurrHeapCount;
		int PeakNumAlloc;
		int CurrNumAlloc;
		int PeakBytesUsed;
		int CurrBytesUsed;
	};

public:
	// Public Interface ------------------------------   for the customers

	// Initialize the mem system
	static Code Initialize();
	static Code Destroy();

	// Create an Heap from the heap system
	static Code HeapCreate(Heap *&newHeap, unsigned int heapSize, const char * const Name);
	static Code HeapCreate(Heap *&newHeap, unsigned int numBlocks, int sizePerBlock, const char * const Name);

	// Create an Heap from the heap system
	static Code HeapDestroy(Heap *inHeap);

	// Information
	static Code GetInfo(Info &info);
	static Code GetHeapByAddr(Heap *&pHeap, void *p);
	static Block *DebugGetGlobalTrackingHead(void);

	// Needed for Testings (Not for Customers)
	static Heap *DebugGetHeapHead(void);

	// -----------------------------------------------------------------------------------
	// Add extra data or methods below this line
	// -----------------------------------------------------------------------------------
	static void FreeBlock(void * p);
	static void* CreateBlock(size_t inSize, Heap *pHeap, Mem::Align align, char *inName, int lineNum);
	void AttachBlock(Block* pBlock);
	void RemoveBlock(Block* pBlock);
private:
	// data -----------------------------------------------------------------------
	//void AttachBlock(Block* pBlock);
	static Mem * Instance();
	Heap	*poHead;
	Block	*pGlobalHead;
	Info	mInfo;
	bool	mInitialized;
	char	pad0;
	char    pad1;
	char    pad2;
};

#endif 

// ---  End of File ---------------
