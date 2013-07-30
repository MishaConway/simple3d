#pragma once
#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>


#include <wrl/client.h>
#include <ppl.h>
#include <ppltasks.h>
#include <agile.h>
using namespace Windows::UI::Core;
#include <d3d11_1.h>

#include "includes\d3dx11effect.h"
//#include <d3dx11effect.h>
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
#include "D3D11ConstantBuffer.h"
//#include <xnamath.h>
#include <DirectXMath.h>
using namespace DirectX;
#include <string>
#include <functional>

#define WINDOWS_STORE_APP 1

#ifdef WINDOWS_STORE_APP
typedef CoreWindow^ DX_WINDOW_TYPE;
typedef DXGI_SWAP_CHAIN_DESC1 SWAP_CHAIN_TYPE;
typedef ID3D11Device1	D3D_DEVICE_TYPE;
typedef ID3D11DeviceContext1 DEVICE_CONTEXT_TYPE;
typedef IDXGIFactory2 DXGI_FACTORY_TYPE;
typedef IDXGIAdapter DXGI_ADAPTER_TYPE;
typedef IDXGISwapChain1 DXGI_SWAP_CHAIN_TYPE;
#else
#define DX_WINDOW_TYPE HWND
#define SWAP_CHAIN_TYPE DXGI_SWAP_CHAIN_DESC
typedef ID3D11Device	DEVICE_TYPE;
typedef ID3D11DeviceContext DEVICE_CONTEXT_TYPE;
typedef IDXGIFactory1 DXGI_FACTORY_TYPE;
typedef IDXGIAdapter1 DXGI_ADAPTER_TYPE;
typedef IDXGISwapChain DXGI_SWAP_CHAIN_TYPE;
#endif

class D3D11GraphicsDevicePrivateInternals
{
friend class D3D11GraphicsDevice;
friend class D3D11Texture;
friend class D3D11Effect;
friend class D3D11Buffer;
friend class D3D11RenderTarget;
friend class D3D11VertexShader;
friend class D3D11PixelShader;
public:
	D3D11GraphicsDevicePrivateInternals();
private:	

	D3D_DEVICE_TYPE* pDevice;
	DEVICE_CONTEXT_TYPE* pDeviceContext;
};


class D3D11GraphicsDevice : public BaseGraphicsDevice
{
public:
	D3D11GraphicsDevice();
	std::string GetRendererType();
	D3D11GraphicsDevicePrivateInternals GetInternals();

	bool Initialize( DX_WINDOW_TYPE hWnd, const unsigned int width, const unsigned int height, const bool debug );
	

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
 DXGI_FACTORY_TYPE* pDXGIFactory;
 D3D11GraphicsDevicePrivateInternals private_internals;
 DXGI_SWAP_CHAIN_TYPE* pSwapChain;
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