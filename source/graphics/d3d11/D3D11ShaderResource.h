#pragma once
#ifdef _WIN32

#include <d3d11_1.h>
//#include <d3dx11.h>
#include "includes/d3dx11effect.h"
#include <string>
#include "../../interfaces/IValidatable.h"

class D3D11ShaderResource : public IValidatable
{
public:
	D3D11ShaderResource();
	D3D11ShaderResource( ID3D11Device* pDevice, const std::string& filepath );
	D3D11ShaderResource( ID3D11Device* pDevice, ID3D11Resource* pResource ); 
	bool IsCreated();
	ID3D11ShaderResourceView* GetResourceView();
private:
	ID3D11ShaderResourceView* pShaderResourceView;
	bool created;
};
#endif