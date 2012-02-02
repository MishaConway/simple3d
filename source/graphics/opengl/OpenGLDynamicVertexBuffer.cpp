#include "OpenGLDynamicVertexBuffer.h"

OpenGLDynamicVertexBuffer::OpenGLDynamicVertexBuffer(){}
OpenGLDynamicVertexBuffer::OpenGLDynamicVertexBuffer( const std::vector<Vertex>& vertices )
{	
	InitializeVertexBuffer( vertices, GL_DYNAMIC_DRAW );
}

OpenGLDynamicVertexBuffer::OpenGLDynamicVertexBuffer( const unsigned int num_vertices )
{
	InitializeVertexBuffer( num_vertices, GL_DYNAMIC_DRAW );		
}

bool OpenGLDynamicVertexBuffer::SetVertices( const std::vector<Vertex>& vertices )
{
	this->vertices = vertices;
	if( vertices.empty() )
		return false;
	else
		return buffer.SetData( (void*) &vertices[0] );	
}