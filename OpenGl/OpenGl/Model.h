#pragma once
#include "sb7.h"
#include "MathEngine.h"
#include <cassert>
class Model
{
public:
	enum BaseModelType
	{
		Cube,
		Pyramid,
		Sphere,
		Diamond,
		Dynamic
	};
	Model() = delete;
	Model(BaseModelType modelType);
	Model(unsigned char* vertData, unsigned char* triData, const unsigned int totalVerts, const unsigned int totalTris);
	~Model();
	
	int getNumVerts();
	int getNumTris();
	void DrawModel();
private:
	void privGenSphere();
	void bindVAO();
	GLuint vao;
	GLuint vbo[2];

	int numVerts;
	int numTris;
};

