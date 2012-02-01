#ifdef _WIN32
#include "D3D11VertexBuffer.h"

D3D11VertexBuffer::D3D11VertexBuffer(){}
D3D11VertexBuffer::D3D11VertexBuffer( const std::vector<Vertex>& vertices )
{		
	InitializeVertexBuffer( vertices, D3D11_USAGE_DEFAULT );
}

void D3D11VertexBuffer::InitializeVertexBuffer( const std::vector<Vertex>& vertices, D3D11_USAGE usage )
{
	this->vertices = vertices;
	num_vertices = vertices.size();
	void* pInitialData = nullptr;
	if( !vertices.empty() )
		pInitialData = (void*) &vertices[0]; 
	buffer = D3D11Buffer( pInitialData, num_vertices * sizeof(Vertex), D3D11_BIND_VERTEX_BUFFER, usage );  
}

void D3D11VertexBuffer::InitializeVertexBuffer( const unsigned int num_vertices, D3D11_USAGE usage )
{
	this->num_vertices = num_vertices;
	buffer = D3D11Buffer( nullptr, num_vertices * sizeof(Vertex), D3D11_BIND_VERTEX_BUFFER, usage );  
}

bool D3D11VertexBuffer::IsValid()
{
	return buffer.IsValid();
}

D3D_PRIMITIVE_TOPOLOGY D3D11VertexBuffer::GetD3D11PrimitiveType()
{
	switch( primitive_type )
	{
	case PrimitiveType::TRIANGLELIST:
		return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	case PrimitiveType::LINELIST:
		return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
	}
	return (D3D_PRIMITIVE_TOPOLOGY)-1;
}

#endif
	