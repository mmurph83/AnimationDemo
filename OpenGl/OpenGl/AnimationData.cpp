#include "AnimationData.h"



AnimationData::AnimationData(int stacks, void* data)
{
	//pData = (StackData*)data;
	totalStacks = stacks;
	void* temp = data;
	int layer = ((int*)temp)[0];
	temp = ((char*)temp) + 32;
	int frames = ((int*)temp)[0];
	temp = ((char*)temp) + 4;
	int bones = ((int*)temp)[0];
	temp = ((char*)temp) - 4;
	//temp = ((char*)temp) + 24;
	FrameData* pframe = (FrameData*)temp;
	int size = 0;
	float v = 0.0f;
	temp = ((char*)temp);
	size = (frames *9 * 4 * bones) + (2*frames*4) + 32;
	size *= layer;
	size += 32;
	//temp = ((char*)data) + size;
	
	//temp = ((char*)temp) + size;
	//layer = ((int*)temp)[0];
	temp = data;
	for (int i = 0; i < stacks; i++)
	{
		layer = ((int*)temp)[0];
		temp = ((char*)temp) + 32;
		frames = ((int*)temp)[0];
		temp = ((char*)temp) + 4;
		bones = ((int*)temp)[0];
		size = (frames * 9 * 4 * bones) + (2 * frames * 4) + 32;
		size *= layer;
		size += 32;
		temp = ((char*)temp) - 36;
		AddStackData(new StackData(temp), i);
		temp = ((char*)data) + size;
		data = temp;
	}
}


AnimationData::~AnimationData()
{
}

void AnimationData::AddStackData(StackData* pStack, int id)
{
	if (pHead == nullptr)
	{
		pHead = new StackList();
		pHead->pData = pStack;
		pHead->pNext = nullptr;
		pHead->index = id;
	}
	else
	{
		StackList* pTemp = pHead;
		while (pTemp->pNext != nullptr)
		{
			pTemp = pTemp->pNext;
		}
		pTemp->pNext = new StackList();
		pTemp = pTemp->pNext;
		pTemp->pData = pStack;
		pTemp->pNext = nullptr;
		pTemp->index = id;
	}
}
int AnimationData::GetStackCount()
{
	return totalStacks;
}

StackData* AnimationData::GetStackData(int id)
{
	StackList* pTemp = pHead;
	while (pTemp->pNext != nullptr && pTemp->index != id)
	{
		pTemp = pTemp->pNext;
	}
	return pTemp->pData;
}