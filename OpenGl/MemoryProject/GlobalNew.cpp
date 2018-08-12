//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2017
// Optimized C++
//----------------------------------------------------------------------------- 

#include "Mem.h"
#include "Heap.h"
#include "GlobalNew.h"

#define STUB_PLEASE_REPLACE(x) (x)

// operator new
void * operator new( size_t inSize, Heap *pHeap, Mem::Align align, char *inName, int lineNum )
{
	STUB_PLEASE_REPLACE(inSize);
	STUB_PLEASE_REPLACE(pHeap);
	STUB_PLEASE_REPLACE(align);
	STUB_PLEASE_REPLACE(inName);
	STUB_PLEASE_REPLACE(lineNum);
	return Mem::CreateBlock(inSize, pHeap, align, inName, lineNum);
}

void * operator new( size_t inSize )
{
	void *p = malloc( inSize );
	return p;
}

// operator delete
void operator delete( void *p, Heap *pHeap, Mem::Align align, char *inName, int lineNum)
{
	STUB_PLEASE_REPLACE(p);
	STUB_PLEASE_REPLACE(pHeap);
	STUB_PLEASE_REPLACE(align);
	STUB_PLEASE_REPLACE(inName);
	STUB_PLEASE_REPLACE(lineNum);
}

// operator delete
void operator delete( void *p )
{
	Mem::FreeBlock(p);
	//HeapFree(GetProcessHeap(), 0, p);
	//HeapDestroy(p);
}

void * operator new[]( size_t inSize ,Heap *pHeap, Mem::Align align, char *inName, int lineNum )
{
	STUB_PLEASE_REPLACE(inSize);
	STUB_PLEASE_REPLACE(pHeap);
	STUB_PLEASE_REPLACE(align);
	STUB_PLEASE_REPLACE(inName);
	STUB_PLEASE_REPLACE(lineNum);
	return Mem::CreateBlock(inSize, pHeap, align, inName, lineNum);
}

void operator delete[]( void *p,Heap *pHeap, Mem::Align align, char *inName, int lineNum )
{
	STUB_PLEASE_REPLACE(p);
	STUB_PLEASE_REPLACE(pHeap);
	STUB_PLEASE_REPLACE(align);
	STUB_PLEASE_REPLACE(inName);
	STUB_PLEASE_REPLACE(lineNum);
	HeapDestroy(p);
}

void * operator new[]( size_t inSize )
{
	void *p = malloc( inSize );
	return p;
}

void operator delete[]( void *p )
{
	Mem::FreeBlock(p);
}

// ---  End of File ---------------
