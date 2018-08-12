#include "CameraObject.h"
#include <vmath.h>


CameraObject::CameraObject():vRight(1.0f,0.0f,0.0f),vUp(0.0f,1.0f,0.0f),vLookAt(0.0f,0.0f,1.0f),vDir(), vPos()
{
	float tmpAspect = (float)800.0f / (float)600.0f;
	float fovy = 35.0f;
	float near = 1.0f;
	float far = 15500.0f;
	vmath::mat4 tmpMatrix = vmath::perspective(fovy, tmpAspect, near, far);
	SetPerspective(fovy, tmpAspect, near, far);
	mProj = (Matrix &)tmpMatrix;
	privSetViewMatrix();
	privCalcPlaneHeightWidth();
	privCalcFrustumVerts();
	privCalcFrustumCollisionNormals();
}
void CameraObject::privUpdateProjectionMatrix(void)
{
	this->mProj[m0] = 2.0f * this->nearDist / this->nearWidth;
	this->mProj[m1] = 0.0f;
	this->mProj[m2] = 0.0f;
	this->mProj[m3] = 0.0f;
		  
	this->mProj[m4] = 0.0f;
	this->mProj[m5] = 2.0f * this->nearDist / this->nearHeight;
	this->mProj[m6] = 0.0f;
	this->mProj[m7] = 0.0f;
		  
	this->mProj[m8] = 0.0f;
	this->mProj[m9] = 0.0f;
	this->mProj[m10] = (this->farDist + this->nearDist) / (this->nearDist - this->farDist);
	this->mProj[m11] = -1.0f;
		  
	this->mProj[m12] = 0.0f;
	this->mProj[m13] = 0.0f;
	this->mProj[m14] = (2.0f * this->farDist * this->nearDist) / (this->nearDist - this->farDist);
	this->mProj[m15] = 0.0f;
};

CameraObject::~CameraObject()
{
}

void CameraObject::SetPerspective(const float Fovy, const float Aspect, const float NearDist, const float FarDist)
{
	fovy = Fovy;
	aspect = Aspect;
	nearDist = NearDist;
	farDist = FarDist;
}

void CameraObject::privCalcPlaneHeightWidth(void)
{

	this->nearHeight = 2.0f * tanf((this->fovy * MATH_PI / 180.0f) * .5f) * this->nearDist;
	this->nearWidth = this->nearHeight * this->aspect;

	this->farHeight = 2.0f * tanf((this->fovy * MATH_PI / 180.0f) * .5f) * this->farDist;
	this->farWidth = this->farHeight * this->aspect;

}

void CameraObject::SetCamPos(Vect& pos)
{
	vPos = pos;
	privSetViewMatrix();
}
void CameraObject::SetOrientation(const Vect &inUp, const Vect &inLookAt)
{
	this->vLookAt = inLookAt;

	// Point out of the screen into your EYE
	this->vDir = vPos - inLookAt;
	this->vDir.norm();

	// Clean up the vectors (Right hand rule)
	this->vRight = inUp.cross(this->vDir);
	this->vRight.norm();

	this->vUp = this->vDir.cross(this->vRight);
	this->vUp.norm();

	this->vPos = vPos;
	privSetViewMatrix();

}

void CameraObject::UpdateCamera()
{
	privSetViewMatrix();
	privCalcPlaneHeightWidth();
	privCalcFrustumVerts();
	privCalcFrustumCollisionNormals();
	privUpdateProjectionMatrix();
}
Matrix &CameraObject::getView()
{
	return mView;
}
Matrix & CameraObject::getProj()
{
	return mProj;
}
Vect & CameraObject::getPos()
{
	return vPos;
}
Vect & CameraObject::GetCameraDir()
{
	return vDir;
}

void CameraObject::privSetViewMatrix()
{
	this->mView[m0] = this->vRight[x];
	this->mView[m1] = this->vUp[x];
	this->mView[m2] = this->vDir[x];
	this->mView[m3] = 0.0f;
		  
	this->mView[m4] = this->vRight[y];
	this->mView[m5] = this->vUp[y];
	this->mView[m6] = this->vDir[y];
	this->mView[m7] = 0.0f;
		  
	this->mView[m8] = this->vRight[z];
	this->mView[m9] = this->vUp[z];
	this->mView[m10] = this->vDir[z];
	this->mView[m11] = 0.0f;
		  
	// ChamViewis (dot with the basis vectors)
	this->mView[m12] = -vPos.dot(vRight);
	this->mView[m13] = -vPos.dot(vUp);
	this->mView[m14] = -vPos.dot(vDir);
	this->mView[m15] = 1.0f;
}
void CameraObject::setOrientAndPosition(const Vect &inUp, const Vect &inLookAt, const Vect &inPos)
{
	// Remember the up, lookAt and right are unit, and are perpendicular.
	// Treat lookAt as king, find Right vect, then correct Up to insure perpendiculare.
	// Make sure that all vectors are unit vectors.

	this->vLookAt = inLookAt;

	// Point out of the screen into your EYE
	this->vDir = inPos - inLookAt;
	this->vDir.norm();

	// Clean up the vectors (Right hand rule)
	this->vRight = inUp.cross(this->vDir);
	this->vRight.norm();

	this->vUp = this->vDir.cross(this->vRight);
	this->vUp.norm();

	this->vPos = inPos;
};
void CameraObject::getNearTopLeft(Vect &vOut) const
{
	vOut = this->nearTopLeft;
}
void CameraObject::getNearTopRight(Vect &vOut) const
{
	vOut = this->nearTopRight;
}
void CameraObject::getNearBottomLeft(Vect &vOut)const
{
	vOut = this->nearBottomLeft;
}
void CameraObject::getNearBottomRight(Vect &vOut) const
{
	vOut = this->nearBottomRight;
}
void CameraObject::getFarTopLeft(Vect &vOut) const
{
	vOut = this->farTopLeft;
}
void CameraObject::getFarTopRight(Vect &vOut) const
{
	vOut = this->farTopRight;
}
void CameraObject::getFarBottomLeft(Vect &vOut) const
{
	vOut = this->farBottomLeft;
}
void CameraObject::getFarBottomRight(Vect &vOut)const
{
	vOut = this->farBottomRight;
}

void CameraObject::privCalcFrustumCollisionNormals(void)
{
	// Normals of the frustum around nearTopLeft
	Vect A = this->nearBottomLeft - this->nearTopLeft;
	Vect B = this->nearTopRight - this->nearTopLeft;
	Vect C = this->farTopLeft - this->nearTopLeft;

	this->frontNorm = A.cross(B);
	this->frontNorm.norm();

	this->leftNorm = C.cross(A);
	this->leftNorm.norm();

	this->topNorm = B.cross(C);
	this->topNorm.norm();

	// Normals of the frustum around farBottomRight
	A = this->farBottomLeft - this->farBottomRight;
	B = this->farTopRight - this->farBottomRight;
	C = this->nearBottomRight - this->farBottomRight;

	this->backNorm = A.cross(B);
	this->backNorm.norm();

	this->rightNorm = B.cross(C);
	this->rightNorm.norm();

	this->bottomNorm = C.cross(A);
	this->bottomNorm.norm();
}

void CameraObject::privCalcFrustumVerts(void)
{
	// Top Left corner and so forth.  In this form to see the pattern
	// Might be confusing (remember the picture) vDir goes from screen into your EYE
	// so distance from the eye is "negative" vDir
	this->nearTopLeft = this->vPos - this->vDir * this->nearDist + 0.5f * this->vUp * this->nearHeight  - 0.5f * this->vRight * this->nearWidth;
	this->nearTopRight = this->vPos - this->vDir * this->nearDist + 0.5f *this->vUp * this->nearHeight + 0.5f *this->vRight * this->nearWidth;
	this->nearBottomLeft = this->vPos - this->vDir * this->nearDist - 0.5f *this->vUp * this->nearHeight  - 0.5f *this->vRight * this->nearWidth;
	this->nearBottomRight = this->vPos - this->vDir * this->nearDist - 0.5f *this->vUp * this->nearHeight + 0.5f *this->vRight * this->nearWidth;
	this->farTopLeft = this->vPos - this->vDir * this->farDist + 0.5f *this->vUp * this->farHeight  - 0.5f *this->vRight * this->farWidth;
	this->farTopRight = this->vPos - this->vDir * this->farDist + 0.5f *this->vUp * this->farHeight  + 0.5f *this->vRight * this->farWidth;
	this->farBottomLeft = this->vPos - this->vDir * this->farDist - 0.5f *this->vUp * this->farHeight - 0.5f *this->vRight * this->farWidth;
	this->farBottomRight = this->vPos - this->vDir * this->farDist - 0.5f *this->vUp * this->farHeight  + 0.5f *this->vRight * this->farWidth ;
}

Vect CameraObject::GetTopNorm()
{
	return topNorm;
}
Vect CameraObject::GetBottomNorm()
{
	return bottomNorm;
}
Vect CameraObject::GetLeftNorm()
{
	return leftNorm;
}
Vect CameraObject::GetRightNorm()
{
	return rightNorm;
}
Vect CameraObject::GetBackNorm()
{
	return backNorm;
}
Vect CameraObject::GetFrontNorm()
{
	return frontNorm;
}