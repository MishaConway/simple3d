#include "D3D11Shader.h"
#include "D3D11GraphicsDevice.h"

D3D11GraphicsDevice* D3D11Effect::pGraphicsDevice = nullptr;

D3D11VertexShader::D3D11VertexShader( const std::string& name, std::vector<unsigned char> bytecode )
{
	this->name = name;
	if( FAILED( pGraphicsDevice->GetInternals().pDevice->CreateVertexShader( &bytecode[0], bytecode.size(), nullptr, &pVertexShader ) ) )
		printf( "unable to create vertex shader..\n" );
}

D3D11PixelShader::D3D11PixelShader( const std::string& name, std::vector<unsigned char> bytecode )
{
	this->name = name;
	if( FAILED( pGraphicsDevice->GetInternals().pDevice->CreatePixelShader( &bytecode[0], bytecode.size(), nullptr, &pPixelShader ) ) )
		printf( "unable to create pixel shader..\n" );
}