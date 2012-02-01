#pragma once

#ifdef _WIN32
#include <Windows.h>
#include "glew/glew.h"
#pragma comment(lib, "glew32.lib")  
#pragma comment(lib, "opengl32.lib") 
#endif

#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR || TARGET_OS_IPAD || TARGET_OS_IPAD_SIMULATOR
#include <OpenGLES/ES1/gl.h>
#endif

