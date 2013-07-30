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
public:
	D3D11Shader();
protected:
	static D3D11GraphicsDevice* pGraphicsDevice;
};

class D3D11VertexShader : public D3D11Shader
{
public:
	D3D11VertexShader( const std::string& name, std::vector<unsigned char> bytecode );

private:
	ID3D11VertexShader* pVertexShader;
};

class D3D11PixelShader : public D3D11Shader
{
public:
	D3D11PixelShader( const std::string& name, std::vector<unsigned char> bytecode );

private:
	ID3D11PixelShader* pPixelShader;
};