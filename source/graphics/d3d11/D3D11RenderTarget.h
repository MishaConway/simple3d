#pragma once
#ifdef _WIN32

#include "D3D11Texture.h"
#include "D3D11Effect.h"
#include "../BaseRenderTarget.h"
#include <vector>


//class Effect;
class D3D11RenderTarget : public BaseRenderTarget
{
friend class D3D11GraphicsDevice;
public:
	D3D11RenderTarget();
	D3D11RenderTarget( const unsigned int width, const unsigned int height );
	D3D11Texture& GetTexture();
	bool HorizontalBlur( D3D11RenderTarget pRenderTargetWritten );
	bool VerticalBlur( D3D11RenderTarget pRenderTargetWritten );
	
	ID3D11RenderTargetView* GetRenderTargetView();
	ID3D11DepthStencilView* GetDepthStencilView();

	void Free();
	D3D11Texture GrabTextureAndFree();
private:
	bool Blur( D3D11RenderTarget pRenderTargetWritten, const std::string& blur_technique );
private:
	ID3D11RenderTargetView* pRenderTargetView;
	ID3D11Texture2D *pDepthStencilBuffer;
	ID3D11DepthStencilView* pDepthStencilView;
	D3D11Texture tex;
	static D3D11GraphicsDevice* pGraphicsDevice;
};
#endif