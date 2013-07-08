#ifdef _WIN32
#include "D3D11ShaderResource.h"

D3D11ShaderResource::D3D11ShaderResource(){created = false;}

D3D11ShaderResource::D3D11ShaderResource( ID3D11Device* pDevice, const std::string& filepath )
{
	created = true;
	//valid = SUCCEEDED( D3DX11CreateShaderResourceViewFromFile( pDevice, filepath.c_str(), NULL, NULL, &pShaderResourceView, NULL ) );
	valid = false;
}

D3D11ShaderResource::D3D11ShaderResource( ID3D11Device* pDevice, ID3D11Resource* pResource )
{
	created = true;
	valid = SUCCEEDED( pDevice->CreateShaderResourceView( pResource, 0, &pShaderResourceView ) );
}

ID3D11ShaderResourceView* D3D11ShaderResource::GetResourceView()
{	
	return pShaderResourceView;
}

bool D3D11ShaderResource::IsCreated()
{
	return created;
}
#endif