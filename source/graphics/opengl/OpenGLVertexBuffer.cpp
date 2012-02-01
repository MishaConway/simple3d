#include "OpenGLVertexBuffer.h"

OpenGLVertexBuffer::OpenGLVertexBuffer(){}
OpenGLVertexBuffer::OpenGLVertexBuffer( const std::vector<Vertex>& vertices )
{		
	InitializeVertexBuffer( vertices, GL_STATIC_DRAW_ARB );
}

void OpenGLVertexBuffer::InitializeVertexBuffer( const std::vector<Vertex>& vertices, GLenum usage )
{
	this->vertices = vertices;
	num_vertices = vertices.size();
	void* pInitialData = nullptr;
	if( !vertices.empty() )
		pInitialData = (void*) &vertices[0]; 
	buffer = OpenGLBuffer( pInitialData, num_vertices * sizeof(Vertex), GL_ARRAY_BUFFER_ARB, usage );  
}

void OpenGLVertexBuffer::InitializeVertexBuffer( const unsigned int num_vertices, GLenum usage )
{
	this->num_vertices = num_vertices;
	buffer = OpenGLBuffer( nullptr, num_vertices * sizeof(Vertex), GL_ARRAY_BUFFER_ARB, usage );  
}

bool OpenGLVertexBuffer::IsValid()
{
	return buffer.IsValid();
}
	
GLenum OpenGLVertexBuffer::GetOpenGLPrimitiveType()
{
	switch( primitive_type )
	{
	case PrimitiveType::TRIANGLELIST:
		return GL_TRIANGLES;
	case PrimitiveType::LINELIST:
		return GL_LINES;
	}
	return -1;
}