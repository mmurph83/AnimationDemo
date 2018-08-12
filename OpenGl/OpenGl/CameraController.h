#pragma once
#include "CameraManager.h"
#include "Keyboard.h"
class CameraController
{
public:
	CameraController();
	~CameraController();
	void Update(float time);
private:
	CameraObject* pCam;
	bool isPressed = false;
};

