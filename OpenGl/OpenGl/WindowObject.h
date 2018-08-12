#pragma once
#ifdef WIN32
#pragma once
#define _CRT_SECURE_NO_WARNINGS 1

#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#else
#include <unistd.h>
#define Sleep(t) sleep(t)
#endif

#include "GL/gl3w.h"

#define GLFW_NO_GLU 1
#define GLFW_INCLUDE_GLCOREARB 1

#include "GLFW/glfw3.h"

#include "sb7ext.h"

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <cassert>
class WindowObject
{
public:
	static WindowObject* instance();
	WindowObject() = delete;
	WindowObject(const char* name, int Width, int Height);
	~WindowObject();
	
	struct APPINFO
	{
		char title[128];
		int windowWidth;
		int windowHeight;
		int majorVersion;
		int minorVersion;
		int samples;
		union
		{
			struct
			{
				unsigned int    fullscreen : 1;
				unsigned int    vsync : 1;
				unsigned int    cursor : 1;
				unsigned int    stereo : 1;
				unsigned int    debug : 1;
				unsigned int    robust : 1;
			};
			unsigned int        all;
		} flags;
	};
	GLFWwindow* getWindow();
private:
	void privInitializeWindow();
	void privPreInitialize();
	APPINFO info;
	GLFWwindow* window;
};

