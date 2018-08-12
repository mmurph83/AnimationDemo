#pragma once
#include "FrameData.h"
#include "MathEngine.h"
class FrameData;
class Vect;
class Quat;
class LayerData
{
public:
	LayerData(void* data);
	~LayerData();
	void GetFrameData(int frame, int bone, Vect& pos, Quat& rot, Vect& scale);
	void GetFramesTimeIsBetween(int time, int& a, int & b);
	int GetTimeAtFrame(int frame);
	int GetTotalFrames();
private:
	int frames;
	int bones;
	char name[24];
	void* pData;
};

