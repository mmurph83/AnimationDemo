#pragma once
#include "MathEngine.h"
#include <sb7.h>
class LightObject
{
public:
	LightObject();
	LightObject(Vect& pos, Vect& color);
	~LightObject();
	void setPos(Vect& pos);
	void setColor(Vect& color);
	Vect &Pos();
	Vect &Color();
private:
	Vect lightPos;
	Vect lightColor;
};

