#pragma once

class FrameData
{
public:
	FrameData(void* data);
	~FrameData();
	void GetPosition(int bone, float &x, float &y, float &z);
	void GetScale(int bone, float &x, float &y, float &z);
	void GetRotation(int bone, float &x, float &y, float &z);

private:
		float tX, tY, tZ;
		float rX, rY, rZ;
		float sX, sY, sZ;

};

