#pragma once

#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
#include "glew/glew.h"
#pragma comment(lib, "glew32.lib")  
#pragma comment(lib, "opengl32.lib") 
#endif

//find a better way to do this
#if defined(__APPLE__) || defined(__APPLE_CC__)
#define TARGET_IOS 1
#endif

#if TARGET_IOS || TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR || TARGET_OS_IPAD || TARGET_OS_IPAD_SIMULATOR
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#define glMapBuffer glMapBufferOES
#define glUnmapBuffer glUnmapBufferOES
#define GL_WRITE_ONLY GL_WRITE_ONLY_OES
#endif

