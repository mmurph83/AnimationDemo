#pragma once
#include "MathEngine.h"
class CameraObject
{
public:
	CameraObject();
	~CameraObject();
	void SetCamPos(Vect& pos);
	void SetOrientation(const Vect &inUp, const Vect &inLookAt);
	void UpdateCamera();
	void SetPerspective(const float Fovy, const float Aspect, const float NearDist, const float FarDist);
	void privCalcPlaneHeightWidth(void);
	Matrix &getView();
	Matrix &getProj();
	Vect &getPos();
	Vect &GetCameraDir();

	void getNearTopLeft(Vect &vOut) const;
	void getNearTopRight(Vect &vOut) const;
	void getNearBottomLeft(Vect &vOut) const;
	void getNearBottomRight(Vect &vOut) const;
	void getFarTopLeft(Vect &vOut) const;
	void getFarTopRight(Vect &vOut) const;
	void getFarBottomLeft(Vect &vOut) const;
	void getFarBottomRight(Vect &vOut) const;

	Vect GetTopNorm();
	Vect GetBottomNorm();
	Vect GetLeftNorm();
	Vect GetRightNorm();
	Vect GetBackNorm();
	Vect GetFrontNorm();
	void setOrientAndPosition(const Vect &inUp, const Vect &inLookAt, const Vect &inPos);
private:
	void privSetViewMatrix();
	void privCalcFrustumCollisionNormals(void);
	void privCalcFrustumVerts(void);
	void privUpdateProjectionMatrix();
	Matrix mView;
	Matrix mProj;
	Vect	vUp;
	Vect	vDir;
	Vect	vRight;  // derived by up and dir
	Vect	vPos;

	Vect	vLookAt;

	Vect	nearTopLeft;
	Vect	nearTopRight;
	Vect	nearBottomLeft;

	Vect	nearBottomRight;
	Vect	farTopLeft;
	Vect	farTopRight;
	Vect	farBottomLeft;

	Vect	farBottomRight;

	Vect	frontNorm;
	Vect	backNorm;
	Vect	rightNorm;

	Vect	leftNorm;
	Vect	topNorm;
	Vect	bottomNorm;


	float fovy;
	float aspect;
	float nearDist;
	float farDist;

	float nearWidth;
	float farWidth;
	float nearHeight;
	float farHeight;
};

