#pragma once
#include "MathEngine.h"
#include "PCSNode.h"
#include "GraphicsObject.h"
#include "Texture.h"
class GameObject : public PCSNode
{
public:
	GameObject(GraphicsObject  *graphicsObject);
	~GameObject();
	void Update(float time);
	void Draw();
	void SetPos(Vect* pos);
	Vect GetPos();
	void SetScale(Vect* scale);
	void SetRotX(float x);
	void SetRotY(float y);
	void SetRotZ(float z);
	void SetRotation(Quat* q);
	void setTexture(Texture::Name name);
	void setOrient(Matrix* m);
	Matrix* GetWorld();
	GraphicsObject &GetGraphics();
	int index = 0;
private:
	
	Matrix pWorld;
	Matrix pLocal;
	Matrix orient;
	Vect pPos;
	Vect pScale;
	Quat rot;
	float rotX;
	float rotY;
	float rotZ;
	GraphicsObject  *pGraphicsObject;
	Texture::Name texName;
};

