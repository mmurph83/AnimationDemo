#include "LayerData.h"


LayerData::LayerData(void* data)
{
	frames = ((int*)data)[0];
	bones = ((int*)data)[1];
	data = ((char*)data) + 32;
	pData = data;
	int ta;
	int tb;
	GetFramesTimeIsBetween(34, ta, tb);
}


LayerData::~LayerData()
{
}


void LayerData::GetFrameData(int frame, int bone, Vect& pos, Quat& rot, Vect& scale)
{
	
	float tx, ty, tz;
	float rx, ry, rz;
	float sx, sy, sz;
	int inc = (bones * 9);
	inc *= 4;
	void* temp = (char*)pData;
	if (frame < frames && frame > -1)
	{
		inc *= frame;
		temp = (char*)pData + inc + (8*(frame +1));
	}
	else if(frame >= frames)
	{
		inc *= (frames - 1);
		temp = (char*)pData + inc + 8;
	}

	
	if (bone >= bones)
	{
		bone = bones - 1;
	}
	else if (bone < 0)
	{
		bone = 0;
	}
	temp = (char*)temp + (bone * 9 * 4);
	FrameData* tempFrame = (FrameData*)temp;
	tempFrame->GetPosition(bone, tx, ty, tz);
	tempFrame->GetRotation(bone, rx, ry, rz);
	tempFrame->GetScale(bone, sx, sy, sz);
	pos.set(tx, ty, tz);
	rot = Quat(Rot3AxisType::ROT_XYZ, rx, ry, rz);
	scale.set(sx, sy, sz);
}

void LayerData::GetFramesTimeIsBetween(int time, int& a, int & b)
{
	int i = 0;
	int inc = (bones * 9) + (2);
	inc *= 4;
	int t = 0;
	void* temp = pData;
	int tempframe = ((int*)temp)[0];
	int temptime = ((int*)temp)[1];
	while (i < frames)
	{
		tempframe = ((int*)temp)[0];
		temptime = ((int*)temp)[1];
		if (temptime >= time)
		{
			a = i - 1;
			b = i;
			return;
		}
		temp = (char*)temp + inc;
	}
}

int LayerData::GetTimeAtFrame(int frame)
{
	int inc = (bones * 9) + (2);
	inc *= 4;
	void* temp = (char*)pData;
	if (frame < frames && frame > -1)
	{
		inc *= frame;
		temp = (char*)pData + inc;
	}
	else if (frame >= frames)
	{
		inc *= (frames - 1);
		temp = (char*)pData + inc;
	}
	return ((int*)temp)[1];
}

int LayerData::GetTotalFrames()
{
	return frames;
}