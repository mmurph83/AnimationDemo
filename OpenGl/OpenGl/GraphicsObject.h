#pragma once
#include "ShaderObject.h"
#include "Model.h"
#include "MathEngine.h"
#include "TextureObject.h"
#include "LightObject.h"
#include <sb7.h>

class GraphicsObject
{
public:
	GraphicsObject(ShaderObject * shader, Model * model, LightObject* light);
	GraphicsObject() = delete;
	~GraphicsObject();
	void Update();
	void Render();
	void SetWorld(Matrix* world);
	void SetTexture(Texture::Name name);
	void SetModel(Model* model);
	void SetShader(ShaderObject *pShader);
	void SetColor(Vect& c);
	TextureObject &GetTexureObject();
private:
	ShaderObject * pShaderObject;
	Model* pModel;
	Matrix* pWorld;
	TextureObject* pTextureObject;
	LightObject* pLight;
	Vect* color;
};

