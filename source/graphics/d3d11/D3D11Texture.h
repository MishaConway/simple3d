#pragma once
#ifdef _WIN32

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx11effect.h>
#include <xnamath.h>
#include <string>
#include <map>
#include <vector>
#include <functional>
#include "D3D11ShaderResource.h"
#include "../BaseTexture.h"

class D3D11Texture : public BaseTexture
{
friend class D3D11GraphicsDevice;
public:
	D3D11Texture();
	D3D11Texture( const unsigned int width, const unsigned int height );
	D3D11Texture( const unsigned int width, const unsigned int height, const Color& color );
	D3D11Texture( const unsigned int width, const unsigned int height,  const unsigned int bind_flags, const D3D11_USAGE usage );
	D3D11Texture( const std::string& image_filename );
	D3D11Texture( D3D11Texture a, D3D11Texture b );
	D3D11Texture( D3D11Texture a, D3D11Texture b, D3D11Texture c );
	D3D11Texture( std::vector<D3D11Texture> textures );
	static D3D11Texture FromText( const std::string& text, const Color& text_color, const Color& background_color );
	D3D11Texture Clone( const unsigned width, const unsigned int height );
	bool IsTextureArray();
	virtual bool IsFloatTexture();
	DXGI_FORMAT GetFormat();
	ID3D11Texture2D* GetTex();
	D3D11ShaderResource& GetShaderResource();
	bool SaveToFile( const std::string& filename, const bool save_only_once = false );
protected:
	unsigned char* Map( unsigned int* pPitch );
	void Unmap();
	D3D11_SUBRESOURCE_DATA Read();
private:
	void PrepareTextureArray( std::vector<D3D11Texture> textures );
	void Setup( const unsigned int width, const unsigned int height, const unsigned int bind_flags, const D3D11_USAGE usage );
	void Setup( const unsigned int width, const unsigned int height, const unsigned int bind_flags, const D3D11_USAGE usage, std::vector<D3D11_SUBRESOURCE_DATA> subresource_datas  );
	unsigned int BitsPerPixel( DXGI_FORMAT fmt );
private:
	ID3D11Texture2D* pTex;
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pDeviceContext;
	D3D11_MAPPED_SUBRESOURCE mapped_resource;
	D3D11_TEXTURE2D_DESC texture_description;
	static std::map<ID3D11Texture2D*, D3D11ShaderResource> shader_resources;
	D3D11ShaderResource shader_resource;
private:
	static D3D11GraphicsDevice* pGraphicsDevice;
};

#endif

