#pragma once
#include "StackData.h"
class AnimationData
{
public:
	AnimationData(int stacks, void* data);
	~AnimationData();
	void AddStackData(StackData* pStack, int id);
	int GetStackCount();
	StackData* GetStackData(int id);
private:
	int totalStacks;
	char name[28];
	struct StackList
	{
		int index;
		StackData* pData;
		StackList* pNext;
	};
	StackList* pHead = nullptr;
};

