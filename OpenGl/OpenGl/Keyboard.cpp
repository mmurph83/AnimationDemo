#include <assert.h>

#include "sb7.h"


#include "Keyboard.h"
#include "WindowObject.h"


Keyboard::Keyboard()
{

}

// Use this to read keyboard
bool Keyboard::GetKeyState(AZUL_KEY key)
{
	bool value;


	if (glfwGetKey(WindowObject::instance()->getWindow(),key) == GLFW_PRESS)
	{
		value = true;
	}
	else
	{
		value = false;
	}

	return value;
}
