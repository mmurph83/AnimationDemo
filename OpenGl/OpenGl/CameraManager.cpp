#include "CameraManager.h"



CameraManager::CameraManager(): pCamera(new CameraObject()), pPrev(new CameraObject())
{
}


CameraManager::~CameraManager()
{
}
CameraManager* CameraManager::instance()
{
	static CameraManager* pManager;
	if (pManager == nullptr)
	{
		pManager = new CameraManager();
	}
	return pManager;
}

CameraObject* CameraManager::getCurrentCamera()
{
	return pCamera;
}
void CameraManager::SwapCamera()
{
	CameraObject* swap = pCamera;
	pCamera = pPrev;
	pPrev = swap;
}