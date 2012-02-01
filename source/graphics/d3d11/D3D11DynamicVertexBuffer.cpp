#ifdef _WIN32
#include "D3D11DynamicVertexBuffer.h"

D3D11DynamicVertexBuffer::D3D11DynamicVertexBuffer(){}
D3D11DynamicVertexBuffer::D3D11DynamicVertexBuffer( const std::vector<Vertex>& vertices )
{	
	InitializeVertexBuffer( vertices, D3D11_USAGE_DYNAMIC );
}

D3D11DynamicVertexBuffer::D3D11DynamicVertexBuffer( const unsigned int num_vertices )
{
	InitializeVertexBuffer( num_vertices, D3D11_USAGE_DYNAMIC );		
}

bool D3D11DynamicVertexBuffer::SetVertices( const std::vector<Vertex>& vertices )
{
	this->vertices = vertices;
	if( vertices.empty() )
		return false;
	else
		return buffer.SetData( (void*) &vertices[0] );	
}
#endif