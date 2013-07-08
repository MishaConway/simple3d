#pragma once

#ifdef _WIN32
#include <d3d11_1.h>
//#include <d3dx11.h>
#include <vector>
#include "D3D11VertexFormats.h"
#include "../../interfaces/IValidatable.h"

class D3D11Buffer : public IValidatable
{
friend class D3D11GraphicsDevice;
public:
	D3D11Buffer();
	D3D11Buffer( void* pData, const unsigned int data_size, const unsigned int bind_flags, D3D11_USAGE usage );
	void* Map();
	void Unmap();
	bool SetData( void* pData  );	
protected:
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pDeviceContext;
	ID3D11Buffer* pBuffer;
	D3D11_MAPPED_SUBRESOURCE mapped_resource;
	static D3D11GraphicsDevice* pGraphicsDevice;
};
#endif