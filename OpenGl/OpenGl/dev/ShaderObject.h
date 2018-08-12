#ifndef SHADER_OBJECT_H
#define SHADER_OBJECT_H

#include <sb7.h>
#include "MathEngine.h"

class ShaderObject
{
public:
	enum Name
	{
		COLOR_LIGHT,
		TEXTURE_SIMPLE
	};

public:
	ShaderObject(ShaderObject::Name shaderName, const char * const pShaderBaseFileName);

	ShaderObject() = delete;
	ShaderObject(const ShaderObject &) = delete;
	ShaderObject &operator = (ShaderObject &) = delete;
	virtual ~ShaderObject() = default;

	void SetActive();
	void SendView(Matrix& view);
	void SendProjection(Matrix& proj);
	void SendWorld(Matrix& world);
	GLint GetLocation(const char * pUniformName);

private:
	bool privCreateShader(GLuint &programObject, const char * const pShaderBaseFileName);
	bool privLoadNCompile(GLuint &shaderObject, const char * const pShaderFileName, GLenum shader_type);

public:
	Name	name;
	GLuint	programObject;

};

#endif