#pragma once
#include "LayerData.h"
class Vect;
class Quat;
class StackData
{
public:
	StackData(void* data);
	StackData();
	~StackData();
	int GetTotalLayers();
	LayerData* GetLayer(int layer);
private:
	int totalLayers;
	char name[28];
	struct Layers
	{
		int index;
		LayerData* pData;
		Layers* pNext;
	};
	Layers* pHead = nullptr;
	LayerData* pLayers;
};

