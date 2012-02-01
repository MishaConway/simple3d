#pragma once
#include "OpenGLVertexBuffer.h"

class OpenGLDynamicVertexBuffer : public OpenGLVertexBuffer
{
public:
	OpenGLDynamicVertexBuffer();
	OpenGLDynamicVertexBuffer( const std::vector<Vertex>& vertices );
	OpenGLDynamicVertexBuffer( const unsigned int num_vertices );
	bool SetVertices( const std::vector<Vertex>& vertices );
};