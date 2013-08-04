#include "D3D11Shader.h"
#include "D3D11GraphicsDevice.h"
#include "../../system/FileIO.h"
#include <fstream>

D3D11GraphicsDevice* D3D11Shader::pGraphicsDevice = nullptr;
std::map<std::string, D3D11VertexShader > D3D11VertexShader::vertex_shaders;
std::map<std::string, D3D11PixelShader > D3D11PixelShader::pixel_shaders;

D3D11Shader::D3D11Shader(){}
D3D11Shader::D3D11Shader( const std::string& name, const std::string& path ) : BaseShader( name, path ){}
std::vector<unsigned char> D3D11Shader::ReadSourceFile( const std::string& path )
{
	//check if precompiled bytecode exists
	//{
		std::vector<unsigned char> bytes = File::ReadAllBytes( path + ".cso" );
		return bytes;
	//}
	//else
	//{
		//TODO: load source, compile it into bytecode, return bytecode
	//}
}



D3D11VertexShader::D3D11VertexShader(){}
D3D11VertexShader::D3D11VertexShader( const std::string& name, const std::string& path ) : D3D11Shader( name, path )
{
	if( vertex_shaders.count( name ) )
	{
		*this = vertex_shaders[name];
		printf( "very cool\n" );
	}
	else if( LoadFromFile( path ) )	
	{		
		valid = true;
		vertex_shaders[name] = *this;
	} 
}
void D3D11VertexShader::OnFoundExistingShader( BaseShader& b )
{
	pVertexShader = dynamic_cast<D3D11VertexShader&>(b).pVertexShader;
}

bool D3D11VertexShader::LoadFromFile( const std::string& path )
{
	std::vector<unsigned char> bytecode = ReadSourceFile( path );
	if( !bytecode.empty() )
	{
		if( FAILED( pGraphicsDevice->GetInternals().pDevice->CreateVertexShader( &bytecode[0], bytecode.size(), nullptr, &pVertexShader ) ) )
			printf( "unable to create vertex shader..\n" );
		else
			return true;
	}
	return false;
}

void D3D11VertexShader::Enable()
{
	pGraphicsDevice->GetInternals().pDeviceContext->VSSetShader( pVertexShader, 0, 0 );
}

D3D11PixelShader::D3D11PixelShader(){}
D3D11PixelShader::D3D11PixelShader( const std::string& name, const std::string& path ) : D3D11Shader( name, path )
{
	if( pixel_shaders.count( name ) )
		*this = pixel_shaders[name];
	else if( LoadFromFile( path ) )	
	{		
		valid = true;
		pixel_shaders[name] = *this;
	} 
}

void D3D11PixelShader::OnFoundExistingShader( BaseShader& b )
{
	pPixelShader = dynamic_cast<D3D11PixelShader&>(b).pPixelShader;
}

bool D3D11PixelShader::LoadFromFile( const std::string& path )
{
	std::vector<unsigned char> bytecode = ReadSourceFile( path );
	if( !bytecode.empty() )
	{
		if( FAILED( pGraphicsDevice->GetInternals().pDevice->CreatePixelShader( &bytecode[0], bytecode.size(), nullptr, &pPixelShader ) ) )
			printf( "unable to create pixel shader..\n" );	
		else
			return true;
	}

	return false;
}

void D3D11PixelShader::Enable()
{
	pGraphicsDevice->GetInternals().pDeviceContext->PSSetShader( pPixelShader, 0, 0 );
}

D3D11ShaderProgram::D3D11ShaderProgram( const std::string& name, D3D11VertexShader vertex_shader, D3D11PixelShader fragment_shader )
{
	this->vertex_shader = vertex_shader;
	this->pixel_shader = fragment_shader;
}

void D3D11ShaderProgram::Enable()
{
	vertex_shader.Enable();
	pixel_shader.Enable();
}