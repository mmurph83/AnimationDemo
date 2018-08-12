#include "FrameData.h"



FrameData::FrameData(void* data)
{

}


FrameData::~FrameData()
{
}

void FrameData::GetPosition(int bone, float &x, float &y, float &z)
{
	x = tX;
	y = tY;
	z = tZ;
}
void FrameData::GetScale(int bone, float &x, float &y, float &z)
{
	x = sX;
	y = sY;
	z = sZ;
}
void FrameData::GetRotation(int bone, float &x, float &y, float &z)
{
	x = rX;
	y = rY;
	z = rZ;
}
