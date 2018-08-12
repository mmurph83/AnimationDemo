#include <assert.h>

#include "sb7.h"
#include "Mouse.h"
#include "WindowObject.h"


Mouse::Mouse()
{

}

// Use this to read mouse buttons
bool Mouse::GetKeyState(AZUL_MOUSE button)
{
	bool value;

	if (glfwGetMouseButton(WindowObject::instance()->getWindow(),button)== GLFW_PRESS)
	{
		value = true;
	}
	else
	{
		value = false;
	}
	return value;
}

void Mouse::GetCursor(float &xPos, float &yPos)
{
	// get mouse position
	double x;
	double y;
	glfwGetCursorPos(WindowObject::instance()->getWindow(), &x, &y);
	
	xPos = (float)x;
	yPos = (float)y;
}