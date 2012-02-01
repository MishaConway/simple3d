#ifdef _WIN32
#include "D3D11GraphicsDeviceStateManager.h"

D3D11GraphicsDeviceStateManager::D3D11GraphicsDeviceStateManager(){}
D3D11GraphicsDeviceStateManager::D3D11GraphicsDeviceStateManager( ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext )
{
	this->pDevice = pDevice;
	this->pDeviceContext = pDeviceContext;

		//create default rasterizer state
	D3D11_RASTERIZER_DESC default_rasterizer_desc;
	memset(&default_rasterizer_desc, 0, sizeof(default_rasterizer_desc));
	default_rasterizer_desc.FillMode = D3D11_FILL_WIREFRAME;
	default_rasterizer_desc.FillMode = D3D11_FILL_SOLID;
	default_rasterizer_desc.CullMode = D3D11_CULL_BACK;
	//default_rasterizer_desc.CullMode = D3D11_CULL_NONE;
	default_rasterizer_desc.DepthClipEnable = TRUE;
	default_rasterizer_desc.MultisampleEnable = TRUE;
	pDevice->CreateRasterizerState(&default_rasterizer_desc, &default_rasterizer_state);

	default_rasterizer_desc.CullMode = D3D11_CULL_FRONT;
	pDevice->CreateRasterizerState(&default_rasterizer_desc, &default_rasterizer_state_back_faces);

	//create sprite rasterizer state
	D3D11_RASTERIZER_DESC sprite_rasterizer_desc;
	memset(&sprite_rasterizer_desc, 0, sizeof(sprite_rasterizer_desc));
	sprite_rasterizer_desc.FillMode = D3D11_FILL_SOLID;
	sprite_rasterizer_desc.CullMode = D3D11_CULL_NONE;
	sprite_rasterizer_desc.DepthClipEnable = TRUE;
	sprite_rasterizer_desc.MultisampleEnable = TRUE;
	sprite_rasterizer_desc.AntialiasedLineEnable = TRUE;
	pDevice->CreateRasterizerState(&sprite_rasterizer_desc, &sprite_rasterizer_state);


	D3D11_DEPTH_STENCIL_DESC depth_stencil_desc;
	// Depth test parameters
	depth_stencil_desc.DepthEnable = TRUE;
	depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS;

	// Stencil test parameters
	depth_stencil_desc.StencilEnable = TRUE;
	depth_stencil_desc.StencilReadMask = 0xFF;
	depth_stencil_desc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	depth_stencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depth_stencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	depth_stencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depth_stencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	pDevice->CreateDepthStencilState( &depth_stencil_desc, &default_depth_stencil_state );

	D3D11_DEPTH_STENCIL_DESC sprite_depth_stencil_desc = depth_stencil_desc;
	sprite_depth_stencil_desc.DepthEnable = FALSE;
	sprite_depth_stencil_desc.StencilEnable = FALSE;
	pDevice->CreateDepthStencilState( &sprite_depth_stencil_desc, &sprite_depth_stencil_state );

	D3D11_DEPTH_STENCIL_DESC particle_depth_stencil_desc = depth_stencil_desc;
	particle_depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	pDevice->CreateDepthStencilState( &particle_depth_stencil_desc, &particle_depth_stencil_state );

	D3D11_BLEND_DESC transparency_blend_state_description;
	ZeroMemory( &transparency_blend_state_description, sizeof(transparency_blend_state_description) );
	transparency_blend_state_description.RenderTarget[0].BlendEnable = TRUE;
	transparency_blend_state_description.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	transparency_blend_state_description.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	transparency_blend_state_description.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	transparency_blend_state_description.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	transparency_blend_state_description.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	transparency_blend_state_description.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	transparency_blend_state_description.RenderTarget[0].RenderTargetWriteMask = 0x0f;
	transparency_blend_state_description.AlphaToCoverageEnable = false;
	pDevice->CreateBlendState( &transparency_blend_state_description, &transparency_blend_state );

	D3D11_BLEND_DESC additive_blend_state_description;
	ZeroMemory( &additive_blend_state_description, sizeof(additive_blend_state_description) );
	additive_blend_state_description.RenderTarget[0].BlendEnable = TRUE;
	additive_blend_state_description.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	additive_blend_state_description.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	additive_blend_state_description.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	additive_blend_state_description.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	additive_blend_state_description.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	additive_blend_state_description.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	additive_blend_state_description.RenderTarget[0].RenderTargetWriteMask = 0x0f;
	additive_blend_state_description.AlphaToCoverageEnable = false;
	pDevice->CreateBlendState( &additive_blend_state_description, &additive_blend_state );

	locked = false;
	blend_state_locked = false;
}

void D3D11GraphicsDeviceStateManager::SetDefaultFrontFaceRendering()
{
	if( locked )
		return;
	SetRasterizerState( default_rasterizer_state );
	SetDepthStencilState( default_depth_stencil_state );
}

void D3D11GraphicsDeviceStateManager::SetParticleRendering()
{
	if( locked )
		return;
	SetRasterizerState( sprite_rasterizer_state );
	SetDepthStencilState( particle_depth_stencil_state );
	EnableAdditiveBlending();
}

void D3D11GraphicsDeviceStateManager::SetDefaultBackFaceRendering()
{
	if( locked )
		return;
	SetRasterizerState( default_rasterizer_state_back_faces );
	SetDepthStencilState( default_depth_stencil_state );
}

void D3D11GraphicsDeviceStateManager::SetSpriteRendering( const BlendType::GRAPHICS_DEVICE_STATE_MANAGER_BLEND_TYPE blend_type )
{
	if( locked )
		return;
	SetRasterizerState( sprite_rasterizer_state );
	SetDepthStencilState( sprite_depth_stencil_state );
	SetBlendType( blend_type );
}


void D3D11GraphicsDeviceStateManager::EnableAlphaBlending()
{
	if( locked )
		return;
	SetBlendState( transparency_blend_state );
}

void D3D11GraphicsDeviceStateManager::EnableAdditiveBlending()
{
	if( locked )
		return;
	SetBlendState( additive_blend_state );
}

void D3D11GraphicsDeviceStateManager::DisableBlending()
{
	if( locked )
		return;
	SetDefaultBlendState();
}

void D3D11GraphicsDeviceStateManager::SetDefaultRasterizerState()
{
	if( locked )
		return;
	pDeviceContext->RSSetState(0);
}

void D3D11GraphicsDeviceStateManager::SetRasterizerState( ID3D11RasterizerState* rasterizer_state )
{
	if( locked )
		return;
	pDeviceContext->RSSetState(rasterizer_state);
}

void D3D11GraphicsDeviceStateManager::SetDefaultDepthStencilState()
{
	if( locked )
		return;
	pDeviceContext->OMSetDepthStencilState( 0, 1);	
}

void D3D11GraphicsDeviceStateManager::SetDepthStencilState( ID3D11DepthStencilState* depth_stencil_state )
{
	if( locked )
		return;
	pDeviceContext->OMSetDepthStencilState( depth_stencil_state, 1);	
}

void D3D11GraphicsDeviceStateManager::SetDefaultBlendState()
{
	if( locked || blend_state_locked )
		return;
	pDeviceContext->OMSetBlendState(0, 0, 0xffffffff);
}
	
void D3D11GraphicsDeviceStateManager::SetBlendState( ID3D11BlendState* blend_state )
{
	if( locked || blend_state_locked )
		return;
	float blend_factor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	pDeviceContext->OMSetBlendState( blend_state, 0, 0xffffffff ); 
}
#endif