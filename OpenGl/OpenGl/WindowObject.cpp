#include "WindowObject.h"



WindowObject::WindowObject(const char* name, int Width, int Height)
{
	assert(name);

	strcpy_s(info.title, 128, name);
	info.windowWidth = Width;
	info.windowHeight = Height;

	// DEFAULT OPEN GL support
	info.majorVersion = 4;
	info.minorVersion = 3;

	info.samples = 0;
	info.flags.all = 0;
	info.flags.cursor = 1;

#ifdef _DEBUG
	info.flags.debug = 1;
#endif
	privPreInitialize();
	privInitializeWindow();
}


WindowObject::~WindowObject()
{
}
WindowObject* WindowObject::instance()
{
	static WindowObject* pWindow;
	if (pWindow == nullptr)
	{
		char n[] = "hi";
		pWindow = new WindowObject(n, 600,800);
	}
	return pWindow;
}

void WindowObject::privPreInitialize()
{
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return;
	}
}

void WindowObject::privInitializeWindow()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, info.majorVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, info.minorVersion);

#ifndef _DEBUG
	if (info.flags.debug)
#endif /* _DEBUG */
	{
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	}
	if (info.flags.robust)
	{
		glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, GLFW_LOSE_CONTEXT_ON_RESET);
	}
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, info.samples);
	glfwWindowHint(GLFW_STEREO, info.flags.stereo ? GL_TRUE : GL_FALSE);
	//        if (info.flags.fullscreen)
	//        {
	//            if (info.windowWidth == 0 || info.windowHeight == 0)
	//            {
	//                GLFWvidmode mode;
	//                glfwGetDesktopMode(&mode);
	//                info.windowWidth = mode.Width;
	//                info.windowHeight = mode.Height;
	//            }
	//
	//            glfwOpenWindow(info.windowWidth, info.windowHeight, 8, 8, 8, 0, 32, 0, GLFW_FULLSCREEN);
	//            glfwSwapInterval((int)info.flags.vsync);
	//        }
	//        else
	{
		window = glfwCreateWindow(info.windowWidth, info.windowHeight, info.title, info.flags.fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
		if (!window)
		{
			fprintf(stderr, "Failed to open window\n");
			return;
		}
	}

	glfwMakeContextCurrent(window);

	
	if (!info.flags.cursor)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}

	// info.flags.stereo = (glfwGetWindowParam(GLFW_STEREO) ? 1 : 0);

	gl3wInit();

#ifdef _DEBUG
	fprintf(stderr, "VENDOR: %s\n", (char *)glGetString(GL_VENDOR));
	fprintf(stderr, "VERSION: %s\n", (char *)glGetString(GL_VERSION));
	fprintf(stderr, "RENDERER: %s\n", (char *)glGetString(GL_RENDERER));
#endif

	

}

GLFWwindow* WindowObject::getWindow()
{
	return window;
}