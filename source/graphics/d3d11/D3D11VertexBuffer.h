#ifdef _WIN32

#pragma once
#include "D3D11Buffer.h"
#include <vector>
#include <d3d11_1.h>
//#include <d3dx11.h>
#include "D3D11VertexFormats.h"
#include "../../geo/Geometry.h"
#include "../BaseVertexBuffer.h"

class D3D11VertexBuffer : public BaseVertexBuffer, IValidatable
{
friend class D3D11GraphicsDevice;
public:
	D3D11VertexBuffer();
	D3D11VertexBuffer( const std::vector<Vertex>& vertices );
	virtual bool IsValid();
	D3D_PRIMITIVE_TOPOLOGY GetD3D11PrimitiveType();
protected:
	void InitializeVertexBuffer( const std::vector<Vertex>& vertices, D3D11_USAGE usage );
	void InitializeVertexBuffer( const unsigned int num_vertices, D3D11_USAGE usage );
protected:
	D3D11Buffer buffer;
};

#endif