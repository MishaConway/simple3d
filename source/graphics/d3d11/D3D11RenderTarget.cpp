#ifdef _WIN32
#include "../inc.h"
#include "D3D11RenderTarget.h"
#include "D3D11GraphicsDevice.h"
#include "../../scenegraph/Sprite.h"

D3D11GraphicsDevice* D3D11RenderTarget::pGraphicsDevice;

D3D11RenderTarget::D3D11RenderTarget()
{
	valid = false;
}
	
D3D11RenderTarget::D3D11RenderTarget( const unsigned int width, const unsigned int height )
{
	tex = D3D11Texture( width, height, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT ); 
	//tex.SetTiling( 1, -1 );
	
	// Create the render target view.
	D3D11_RENDER_TARGET_VIEW_DESC render_target_view_desc;
	render_target_view_desc.Format = tex.GetFormat();
	render_target_view_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	render_target_view_desc.Texture2D.MipSlice = 0;
	if( FAILED( pGraphicsDevice->GetInternals().pDevice->CreateRenderTargetView(tex.GetTex(), &render_target_view_desc, &pRenderTargetView) ) )
	{
		printf( "could not create render target view..\n" );
	}

	DXGI_SAMPLE_DESC multisample_settings;
	multisample_settings.Count = 1;
	multisample_settings.Quality = 0;
	
	// Create depth stencil texture
    D3D11_TEXTURE2D_DESC descDepth;
    ZeroMemory( &descDepth, sizeof(descDepth) );
    descDepth.Width = width;
    descDepth.Height = height;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc = multisample_settings;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    if( FAILED(pGraphicsDevice->GetInternals().pDevice->CreateTexture2D( &descDepth, NULL, &pDepthStencilBuffer ) ) )
	{
		printf( "could not create depth stencil buffer texture!" );
	}

    // Create the depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
    ZeroMemory( &descDSV, sizeof(descDSV) );
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
    if(FAILED(pGraphicsDevice->GetInternals().pDevice->CreateDepthStencilView( pDepthStencilBuffer, &descDSV, &pDepthStencilView )))
	{
		printf("could not create depth stencil view!" );
	}
   
	valid = true;
}

void D3D11RenderTarget::Free()
{

}
	
D3D11Texture D3D11RenderTarget::GrabTextureAndFree()
{
	return GetTexture();
}

ID3D11RenderTargetView* D3D11RenderTarget::GetRenderTargetView()
{
	return pRenderTargetView;
}

ID3D11DepthStencilView* D3D11RenderTarget::GetDepthStencilView()
{
	return pDepthStencilView;
}

D3D11Texture& D3D11RenderTarget::GetTexture()
{
	return tex;
}

bool D3D11RenderTarget::HorizontalBlur( D3D11RenderTarget render_target_written )
{
	return Blur( render_target_written, "PostProcessHorizontalBlur" );
}

bool D3D11RenderTarget::VerticalBlur( D3D11RenderTarget render_target_written )
{
	return Blur( render_target_written, "PostProcessVerticalBlur" );
}

bool D3D11RenderTarget::Blur( D3D11RenderTarget pRenderTargetWritten, const std::string& blur_technique )
{
	Effect::GetCurrentEffect().SetFloatArray( "BlurWeights", ComputeGaussianKernel( 9, 400.5f  ) );
	pGraphicsDevice->SetRenderTarget( pRenderTargetWritten );
#if RENDERER == RENDERER_D3D11
	Sprite( GetTexture(), GeoFloat2( 0, 0), GeoFloat2( 2, 2 ), BlendType::NONE, blur_technique ).Render();
#endif
	return true;
}
#endif


