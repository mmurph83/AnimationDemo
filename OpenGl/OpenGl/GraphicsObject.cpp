#include "GraphicsObject.h"
#include "CameraManager.h"


GraphicsObject::GraphicsObject(ShaderObject * shader, Model * model, LightObject* light) : pShaderObject(shader), pModel(model), pTextureObject(new TextureObject()), pLight(light), color(new Vect(0.0f, 1.0f,0.0f))
{

}


GraphicsObject::~GraphicsObject()
{
}

void GraphicsObject::Update()
{

}
void GraphicsObject::Render()
{
	CameraObject* pCam = CameraManager::instance()->getCurrentCamera();
	pTextureObject->SendTexture();
	pShaderObject->SetActive();
	Matrix proj = pCam->getProj();
	Matrix view = pCam->getView();
	pShaderObject->SendProjection(proj);
	pShaderObject->SendView(view);
	pShaderObject->SendWorld(*pWorld);
	
	
	
	//glUniform3fv(this->pShaderObject->GetLocation("vLightPos"), 1, (float *)&pLight->Pos());
	//glUniform4fv(this->pShaderObject->GetLocation("vColor"), 1, (float *)&*color);
	pModel->DrawModel();
	//glEnable(GL_CULL_FACE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void GraphicsObject::SetWorld(Matrix* world)
{
	pWorld = world;
}

void GraphicsObject::SetTexture(Texture::Name name)
{
	pTextureObject->SetTexture(name);
}

TextureObject &GraphicsObject::GetTexureObject()
{
	return *pTextureObject;
}

void GraphicsObject::SetModel(Model* model)
{
	pModel = model;
}

void GraphicsObject::SetShader(ShaderObject *pShader)
{
	pShaderObject = pShader;
}

void GraphicsObject::SetColor(Vect& c)
{
	*color = c;
}