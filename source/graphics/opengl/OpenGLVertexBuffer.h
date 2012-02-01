#pragma once
#include "OpenGLBuffer.h"
#include "../../geo/Geometry.h"
#include "../BaseVertexBuffer.h"

class OpenGLVertexBuffer : public BaseVertexBuffer
{
friend class OpenGLGraphicsDevice;
public:
	OpenGLVertexBuffer();
	OpenGLVertexBuffer( const std::vector<Vertex>& vertices );
	virtual bool IsValid();
	GLenum GetOpenGLPrimitiveType();
protected:
	void InitializeVertexBuffer( const std::vector<Vertex>& vertices, GLenum usage );
	void InitializeVertexBuffer( const unsigned int num_vertices, GLenum usage );
protected:
	OpenGLBuffer buffer;
};