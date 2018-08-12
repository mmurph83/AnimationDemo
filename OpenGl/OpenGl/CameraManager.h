#pragma once
#include "CameraObject.h"
static class CameraManager
{
public:
	CameraManager();
	~CameraManager();
	static CameraManager* instance();
	CameraObject* getCurrentCamera();
	void SwapCamera();
private:
	CameraObject* pCamera;
	CameraObject* pPrev;
};

