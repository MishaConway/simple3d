#pragma once

#include "include_opengl.h"
#include <vector>
#include "../../interfaces/IValidatable.h"

class OpenGLBuffer : public IValidatable
{
friend class OpenGLGraphicsDevice;
public:
	OpenGLBuffer();
	OpenGLBuffer( void* pData, const unsigned int data_size, GLenum buffer_type, GLenum usage );
	void* Map();
	bool Unmap();
	bool SetData( void* pData  );	
protected:
	GLuint vbo_id;
	GLenum vbo_buffer_type, vbo_usage, vbo_access;
	unsigned int data_size;
};