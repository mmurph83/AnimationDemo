#include "GameObject.h"



GameObject::GameObject(GraphicsObject  *graphicsObject) : pGraphicsObject(graphicsObject)
{
	pPos = Vect(0.0f,0.0f,-4.0f);
	pScale = Vect(1.0f, 1.0f, 1.0f);
	pWorld = Matrix();
	rotX = 0.0f;
	rotY = 0.0f;
	rotZ = 0.0f;
	rot = Quat(Rot3AxisType::ROT_XYZ, rotX, rotY, rotZ);
}


GameObject::~GameObject()
{
}

void GameObject::Update(float time)
{
	UNUSED_VAR(time);
	Matrix Scale(SCALE, this->pScale);
	Matrix TransA(TRANS, this->pPos);
	Quat q(Rot3AxisType::ROT_XYZ, rotX, rotY, rotZ);
	//rotZ += time;
	//rotY += time;
	GameObject* g = (GameObject*)GetParent();
	this->pLocal = Scale * rot * TransA;
	if (g != nullptr)
	{
		this->pWorld = this->pLocal * g->pWorld;
	}
	else
	{
		this->pWorld = this->pLocal;
	}
	// world matrix
}

void GameObject::Draw()
{
	pGraphicsObject->SetWorld(&orient);
	pGraphicsObject->Render();
}

void GameObject::SetPos(Vect* pos)
{
	pPos = *pos;
}

Vect GameObject::GetPos()
{
	return pPos;
}

void GameObject::SetScale(Vect* scale)
{
	pScale = *scale;
}

void GameObject::SetRotX(float x)
{
	rotX = x;
}
void GameObject::SetRotY(float y)
{
	rotY = y;
}
void GameObject::SetRotZ(float z)
{
	rotZ = z;
}
void GameObject::SetRotation(Quat* q)
{
	rot = *q;
}

void GameObject::setTexture(Texture::Name name)
{
	texName = name;
	pGraphicsObject->SetTexture(texName);
}

void GameObject::setOrient(Matrix* m)
{
	orient = *m;
}
Matrix* GameObject::GetWorld()
{
	return &pWorld;
}

GraphicsObject &GameObject::GetGraphics()
{
	return *pGraphicsObject;
}