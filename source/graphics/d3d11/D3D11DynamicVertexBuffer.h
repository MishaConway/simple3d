#pragma once

#ifdef _WIN32
#include "D3D11VertexBuffer.h"

class D3D11DynamicVertexBuffer : public D3D11VertexBuffer
{
public:
	D3D11DynamicVertexBuffer();
	D3D11DynamicVertexBuffer( const std::vector<Vertex>& vertices );
	D3D11DynamicVertexBuffer( const unsigned int num_vertices );
	bool SetVertices( const std::vector<Vertex>& vertices );
};
#endif