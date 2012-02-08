#include "OpenGLVertexBuffer.h"
#include "../../std/nullptr.h"

OpenGLVertexBuffer::OpenGLVertexBuffer(){}
OpenGLVertexBuffer::OpenGLVertexBuffer( const std::vector<Vertex>& vertices )
{		
	InitializeVertexBuffer( vertices, GL_STATIC_DRAW );
}

void OpenGLVertexBuffer::InitializeVertexBuffer( const std::vector<Vertex>& vertices, GLenum usage )
{
	this->vertices = vertices;
	num_vertices = vertices.size();
	void* pInitialData = nullptr;
	if( !vertices.empty() )
		pInitialData = (void*) &vertices[0]; 
	buffer = OpenGLBuffer( pInitialData, num_vertices * sizeof(Vertex), GL_ARRAY_BUFFER, usage );  
}

void OpenGLVertexBuffer::InitializeVertexBuffer( const unsigned int num_vertices, GLenum usage )
{
	this->num_vertices = num_vertices;
	buffer = OpenGLBuffer( nullptr, num_vertices * sizeof(Vertex), GL_ARRAY_BUFFER, usage );  
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
    case PrimitiveType::POINTLIST:
        return GL_POINTS;
    case PrimitiveType::LINESTRIP:
        return GL_LINE_STRIP;
    case PrimitiveType::TRIANGLESTRIP:
        return GL_TRIANGLE_STRIP;
	}
	return -1;
}