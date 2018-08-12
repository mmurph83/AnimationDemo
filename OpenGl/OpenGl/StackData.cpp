#include "StackData.h"



StackData::StackData()
{

}

StackData::StackData(void* data)
{
	totalLayers = ((int*)data)[0];
	data = ((char*)data) + 32;
	pHead = new Layers();
	Layers* pTemp = pHead;
	pTemp->pData = new LayerData(data);
	pTemp->index = 0;
	void* temp = data;
	//temp = ((char*)temp) + 32;
	int frames = ((int*)temp)[0];
	temp = ((char*)temp) + 4;
	int bones = ((int*)temp)[0];
	int size = (frames * 9 * 4 * bones) + (2 * frames * 4) + 32;
	data = ((char*)data) + size;
	temp = data;
	for (int i = 1; i < totalLayers; i++)
	{
		frames = ((int*)temp)[0];
		temp = ((char*)temp) + 4;
		bones = ((int*)temp)[0];
		size = (frames * 9 * 4 * bones) + (2 * frames * 4) + 32;
		pTemp->pNext = new Layers();
		pTemp = pTemp->pNext;
		pTemp->pData = new LayerData(data);
		pTemp->pNext = nullptr;
		pTemp->index = i;
		data = ((char*)data) + size;
		temp = data;
	}
}


StackData::~StackData()
{
}

int StackData::GetTotalLayers()
{
	return totalLayers;
}
LayerData* StackData::GetLayer(int layer)
{
	Layers* pTemp = pHead;
	while (pTemp->pNext != nullptr && pTemp->index != layer)
	{
		pTemp = pTemp->pNext;
	}
	return pTemp->pData;
}