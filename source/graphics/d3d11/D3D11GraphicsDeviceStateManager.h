#pragma once
#ifdef _WIN32

#include <d3d11_1.h>
//#include <d3dx11.h>
#include "../BaseGraphicsDeviceStateManager.h"

class D3D11GraphicsDeviceStateManager : public BaseGraphicsDeviceStateManager
{
public:
	D3D11GraphicsDeviceStateManager();
	D3D11GraphicsDeviceStateManager( ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext );
	
	virtual void SetDefaultFrontFaceRendering();
	virtual void SetDefaultBackFaceRendering();
	virtual void SetDefaultFrontAndBackRendering();
	virtual void SetParticleRendering();
	virtual void SetSpriteRendering( const BlendType::GRAPHICS_DEVICE_STATE_MANAGER_BLEND_TYPE blend_type = BlendType::NONE );
	virtual void EnableAlphaBlending();
	virtual void EnableAdditiveBlending();
	virtual void DisableBlending();
private:
	void SetDefaultRasterizerState();
	void SetRasterizerState( ID3D11RasterizerState* rasterizer_state );
	void SetDefaultDepthStencilState();
	void SetDepthStencilState( ID3D11DepthStencilState* depth_stencil_state );
	virtual void SetDefaultBlendState();
	void SetBlendState( ID3D11BlendState* blend_state );
private:
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pDeviceContext;

	ID3D11RasterizerState* default_rasterizer_state;
	ID3D11RasterizerState* default_rasterizer_state_back_faces;
	ID3D11RasterizerState* sprite_rasterizer_state;

	ID3D11DepthStencilState* default_depth_stencil_state;
	ID3D11DepthStencilState* sprite_depth_stencil_state;
	ID3D11DepthStencilState* particle_depth_stencil_state;

	ID3D11BlendState* transparency_blend_state;
	ID3D11BlendState* additive_blend_state;
};
#endif