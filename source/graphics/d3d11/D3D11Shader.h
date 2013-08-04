#pragma once

#include "../BaseShader.h"
#include <vector>
#include <d3d11_1.h>
//#include <d3dx11.h>
#include <DirectXMath.h>
using namespace DirectX;

class D3D11GraphicsDevice;

class D3D11Shader : public BaseShader
{
friend class D3D11GraphicsDevice;
public:
	D3D11Shader();
	D3D11Shader( const std::string& name, const std::string& path );
	virtual void Enable() = 0;
protected:
	std::vector<unsigned char> ReadSourceFile( const std::string& path );
protected:
	static D3D11GraphicsDevice* pGraphicsDevice;
};

class D3D11VertexShader : public D3D11Shader
{
public:
	D3D11VertexShader();
	D3D11VertexShader( const std::string& name, const std::string& path );
	virtual void Enable();

	virtual void OnFoundExistingShader( BaseShader& b );
	virtual bool LoadFromFile( const std::string& filename );
private:
	ID3D11VertexShader* pVertexShader;
	static std::map<std::string, D3D11VertexShader > vertex_shaders;
};

class D3D11PixelShader : public D3D11Shader
{
public:
	D3D11PixelShader();
	D3D11PixelShader( const std::string& name, const std::string& path );
	virtual void Enable();

	virtual void OnFoundExistingShader( BaseShader& b );
	virtual bool LoadFromFile( const std::string& filename );
private:
	ID3D11PixelShader* pPixelShader;
	static std::map<std::string, D3D11PixelShader > pixel_shaders;
};

class D3D11ShaderProgram : public BaseShaderProgram
{
public:
	D3D11ShaderProgram( const std::string& name, D3D11VertexShader vertex_shader, D3D11PixelShader fragment_shader );
	virtual void Enable();
private:
	D3D11VertexShader vertex_shader;
	D3D11PixelShader pixel_shader;
};