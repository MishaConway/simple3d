#pragma once
#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx11effect.h>
#include <D3DCompiler.h>
#include "../BaseGraphicsDevice.h"
#include "D3D11GraphicsDeviceStateManager.h"
#include "D3D11VertexBuffer.h"
#include "D3D11DynamicVertexBuffer.h"
#include "D3D11VertexFormats.h"
#include "D3D11ShaderResource.h"
#include "D3D11TextRenderer.h"
#include "D3D11Texture.h"
#include "D3D11RenderTarget.h"
#include "D3D11Effect.h"
#include <xnamath.h>
#include <string>
#include <functional>

class D3D11GraphicsDevicePrivateInternals
{
friend class D3D11GraphicsDevice;
friend class D3D11Texture;
friend class D3D11Effect;
friend class D3D11Buffer;
friend class D3D11RenderTarget;
public:
	D3D11GraphicsDevicePrivateInternals();
private:	

	ID3D11Device* pDevice;
	ID3D11DeviceContext* pDeviceContext;
};


class D3D11GraphicsDevice : public BaseGraphicsDevice
{
public:
	D3D11GraphicsDevice();
	std::string GetRendererType();
	D3D11GraphicsDevicePrivateInternals GetInternals();

	bool Initialize( HWND hWnd, const unsigned int width, const unsigned int height, const bool debug );

	bool Draw( const unsigned int num_vertices );

	virtual void SetViewport( const unsigned int width, const unsigned int height );
	
	D3D11GraphicsDeviceStateManager& GetStateManager();
	
	virtual void Clear( Color color );
	bool SwapBackBuffer();

	bool    SetVertexBuffer( D3D11VertexBuffer& pVertexBuffer );
	bool    SetVertexBuffer( D3D11DynamicVertexBuffer& pVertexBuffer );
	
	void SetDefaultRenderTarget();
	void SetRenderTarget( D3D11RenderTarget& render_target );
	void SetRenderTarget(D3D11RenderTarget& render_target, const Color& clear_color );
	
	void RenderText( const unsigned int x, const unsigned int y, const unsigned int font_size, const std::string& font_name, const std::string& text );
	void RenderTextToBottomCenter( const unsigned int font_size, const std::string& font_name, const std::string& text );
	GeoFloat2 GetTextSize( const unsigned int font_size, const std::string& font_name, const std::string& text );
private:
	bool    SetVertexBuffer( ID3D11Buffer* pVertexBuffer, const D3D11_PRIMITIVE_TOPOLOGY p  );

private:
 IDXGIFactory1* pDXGIFactory;
 D3D11GraphicsDevicePrivateInternals private_internals;
 IDXGISwapChain* pSwapChain;
 ID3D11Texture2D *pBackBuffer;
 ID3D11Texture2D *pDepthStencilBuffer;
 ID3D11DepthStencilView* pDepthStencilView;
 ID3D11DepthStencilView* pCurrentDepthStencilView;
 ID3D11RenderTargetView* pRenderTargetView;
 ID3D11RenderTargetView* pCurrentRenderTargetView;
 ID3D11InputLayout* pVertexLayout;
 ID3DX11Effect* pEffect;
 D3D11GraphicsDeviceStateManager graphics_device_state_manager;
};
#endif