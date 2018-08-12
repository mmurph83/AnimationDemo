#include "LightObject.h"



LightObject::LightObject() : lightPos(Vect()), lightColor(Vect(1.0f,1.0f,1.0f))
{
}
LightObject::LightObject(Vect& pos, Vect& color) : lightPos(pos), lightColor(color)
{

}

LightObject::~LightObject()
{
}
void LightObject::setPos(Vect& pos)
{
	lightPos = pos;
}
void LightObject::setColor(Vect& color)
{
	lightColor = color;
}

Vect &LightObject::Pos()
{
	return lightPos;
}
Vect &LightObject::Color()
{
	return lightColor;
}