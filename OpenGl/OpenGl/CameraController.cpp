#include "CameraController.h"



CameraController::CameraController()
{
	pCam = CameraManager::instance()->getCurrentCamera();
	pCam->getPos() += Vect(1.0f, 0.0f, 0.0f);
}


CameraController::~CameraController()
{
}

void CameraController::Update(float time)
{
	float speed = 100.0f;
	if (Keyboard::GetKeyState(AZUL_KEY::KEY_0))
	{
		if (!isPressed)
		{
			isPressed = true;
			CameraManager::instance()->SwapCamera();
		}
	}
	else
	{
		isPressed = false;
	}
	if (pCam == CameraManager::instance()->getCurrentCamera())
	{
		if (Keyboard::GetKeyState(AZUL_KEY::KEY_ARROW_LEFT))
		{
			pCam->getPos() -= Vect(time*speed, 0.0f, 0.0f);
		}
		else if (Keyboard::GetKeyState(AZUL_KEY::KEY_ARROW_RIGHT))
		{
			pCam->getPos() += Vect(time*speed, 0.0f, 0.0f);
		}
		if(Keyboard::GetKeyState(AZUL_KEY::KEY_ARROW_DOWN))
		{
			pCam->getPos() -= Vect(0.0f, 0.0f, time*speed);
		}
		else if (Keyboard::GetKeyState(AZUL_KEY::KEY_ARROW_UP))
		{
			pCam->getPos() += Vect(0.0f, 0.0f, time*speed);
		}
	}
}