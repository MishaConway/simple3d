#ifdef _WIN32
#include "D3D11GraphicsDevice.h"
#include <fstream>

D3D11GraphicsDevicePrivateInternals::D3D11GraphicsDevicePrivateInternals()
{
	pDevice = nullptr;
	pDeviceContext = nullptr;
}

D3D11GraphicsDevicePrivateInternals D3D11GraphicsDevice::GetInternals()
{
	return private_internals;
}

D3D11GraphicsDevice::D3D11GraphicsDevice()
{
	pSwapChain = nullptr;
	pBackBuffer = nullptr;
	pRenderTargetView = nullptr;
}

std::string D3D11GraphicsDevice::GetRendererType()
{
	return "D3D11";
}

bool D3D11GraphicsDevice::Initialize( HWND hWnd, const unsigned int width, const unsigned int height, const bool debug )
{	
	if( initialized )
		return true;
	
	D3D11RenderTarget::pGraphicsDevice = this;
	D3D11Texture::pGraphicsDevice = this;
	D3D11Effect::pGraphicsDevice = this;
	D3D11Buffer::pGraphicsDevice = this;
		
	unsigned int device_creation_flags = 0;
	if( debug )
		device_creation_flags = D3D11_CREATE_DEVICE_DEBUG;

	DXGI_SAMPLE_DESC multisample_settings;
	multisample_settings.Count = 1;
	multisample_settings.Quality = 0;

	D3D_FEATURE_LEVEL featureLevel;

	DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory( &sd, sizeof(sd) );
    sd.BufferCount = 1;
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
	sd.SampleDesc = multisample_settings;
    sd.Windowed = TRUE;

	const bool separate_device_and_swap_chain_creation = false;
	if( separate_device_and_swap_chain_creation )
	{
		if( FAILED( CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)& pDXGIFactory) ) )
		{
			printf( "could not create dxgi factory..\n" );
		}

		IDXGIAdapter1 *pAdapter;
		if( FAILED( pDXGIFactory->EnumAdapters1(0, &pAdapter) ) )
		{
			printf( "could not get adapter from factory\n" );
		}
	
		if( FAILED( D3D11CreateDevice( pAdapter, D3D_DRIVER_TYPE_UNKNOWN, 0, device_creation_flags, 0, 0, D3D11_SDK_VERSION, &private_internals.pDevice, &featureLevel, &private_internals.pDeviceContext ) ) )
		{
			printf( "could not create device\n" );
		}

		unsigned num_quality_levels = 0;
		for( unsigned int i = 0; i < 32; i ++ )
		{
			private_internals.pDevice->CheckMultisampleQualityLevels( DXGI_FORMAT_R8G8B8A8_UNORM, i, &num_quality_levels );
			if( num_quality_levels )
			{
				multisample_settings.Count = i;
				multisample_settings.Quality = num_quality_levels - 1;
			}
		}

		sd.SampleDesc = multisample_settings;
		if( FAILED( pDXGIFactory->CreateSwapChain( private_internals.pDevice, &sd, &pSwapChain ) ) )
		{
			printf( "could not create swap chain..\n" );
		}	
	}
	else
	{
		if( FAILED( D3D11CreateDeviceAndSwapChain( NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, device_creation_flags, 0, 0, D3D11_SDK_VERSION, &sd, &pSwapChain, &private_internals.pDevice, &featureLevel, &private_internals.pDeviceContext ) ) )
		{
			printf( "could not create device and swap chain\n" );
			return FALSE;
		}
	}

	 // Create a render target view
    if( FAILED( pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (LPVOID*)&pBackBuffer ) ) )
        return FALSE;
    if( FAILED( private_internals.pDevice->CreateRenderTargetView( pBackBuffer, NULL, &pRenderTargetView ) ) )
		return FALSE;
    pBackBuffer->Release();

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
    if( FAILED(private_internals.pDevice->CreateTexture2D( &descDepth, NULL, &pDepthStencilBuffer ) ) )
	{
		printf( "could not create depth stencil buffer texture!" );
		return false;
	}


    // Create the depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
    ZeroMemory( &descDSV, sizeof(descDSV) );
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
    if(FAILED(private_internals.pDevice->CreateDepthStencilView( pDepthStencilBuffer, &descDSV, &pDepthStencilView )))
	{
		printf("could not create depth stencil view!" );
		return 0;
	}
   
	pCurrentRenderTargetView = pRenderTargetView;
	pCurrentDepthStencilView = pDepthStencilView;
    private_internals.pDeviceContext->OMSetRenderTargets( 1, &pRenderTargetView, pDepthStencilView );

	SetViewport( width, height );

	graphics_device_state_manager = D3D11GraphicsDeviceStateManager( private_internals.pDevice, private_internals.pDeviceContext );

	initialized = true;
	return true;
}

bool D3D11GraphicsDevice::Draw( const unsigned int num_vertices )
{	
	//printf("made it to draw....\n");
	private_internals.pDeviceContext->Draw( num_vertices, 0 ); 
	return true;
}

void D3D11GraphicsDevice::SetViewport( const unsigned int width, const unsigned int height )
{
	BaseGraphicsDevice::SetViewport( width, height );
    private_internals.pDeviceContext->RSSetViewports( 1, (D3D11_VIEWPORT*) &viewport );
}



D3D11GraphicsDeviceStateManager& D3D11GraphicsDevice::GetStateManager()
{
	return graphics_device_state_manager;	
}

void D3D11GraphicsDevice::Clear( Color color )
{
	float ClearColor[4] = { color.GetNormalizedRed(), color.GetNormalizedGreen(), color.GetNormalizedBlue(), color.GetNormalizedAlpha() };
	private_internals.pDeviceContext->ClearRenderTargetView( pCurrentRenderTargetView, ClearColor );
	private_internals.pDeviceContext->ClearDepthStencilView( pCurrentDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0, 0 );	
}

bool D3D11GraphicsDevice::SwapBackBuffer()
{
	pSwapChain->Present( 0, 0 );
	return true;
}

bool D3D11GraphicsDevice::SetVertexBuffer( D3D11VertexBuffer& vertexBuffer )
{
	return SetVertexBuffer( vertexBuffer.buffer.pBuffer, vertexBuffer.GetD3D11PrimitiveType() );
}

bool D3D11GraphicsDevice::SetVertexBuffer( D3D11DynamicVertexBuffer& vertexBuffer )
{
	return SetVertexBuffer( vertexBuffer.buffer.pBuffer, vertexBuffer.GetD3D11PrimitiveType() );
}

bool D3D11GraphicsDevice::SetVertexBuffer( ID3D11Buffer* pVertexBuffer, const D3D11_PRIMITIVE_TOPOLOGY p )
{
	UINT stride = sizeof( Vertex );
	UINT offset = 0;
	private_internals.pDeviceContext->IASetVertexBuffers( 0, 1, &pVertexBuffer, &stride, &offset );
	private_internals.pDeviceContext->IASetPrimitiveTopology( p );
	return true;
}

void D3D11GraphicsDevice::SetDefaultRenderTarget()
{
	pCurrentRenderTargetView = pRenderTargetView;
	pCurrentDepthStencilView = pDepthStencilView;
	private_internals.pDeviceContext->OMSetRenderTargets( 1, &pRenderTargetView, pDepthStencilView );
}

void D3D11GraphicsDevice::SetRenderTarget( D3D11RenderTarget& render_target )
{
	pCurrentRenderTargetView = render_target.GetRenderTargetView();
	pCurrentDepthStencilView = render_target.GetDepthStencilView();
	private_internals.pDeviceContext->OMSetRenderTargets( 1, &pCurrentRenderTargetView, pCurrentDepthStencilView ); 
}

void D3D11GraphicsDevice::SetRenderTarget( D3D11RenderTarget& render_target, const Color& clear_color )
{
	SetRenderTarget( render_target );
	Clear( clear_color );
}

void D3D11GraphicsDevice::RenderText( const unsigned int x, const unsigned int y, const unsigned int font_size, const std::string& font_name, const std::string& text )
{
	D3D11TextRenderer( private_internals.pDevice, private_internals.pDeviceContext, font_name, font_size ).Render( x, y, text );	
}
	
void D3D11GraphicsDevice::RenderTextToBottomCenter( const unsigned int font_size, const std::string& font_name, const std::string& text )
{
	D3D11TextRenderer( private_internals.pDevice, private_internals.pDeviceContext, font_name, font_size ).RenderBottomCenter( viewport, text );
}

GeoFloat2 D3D11GraphicsDevice::GetTextSize( const unsigned int font_size, const std::string& font_name, const std::string& text )
{
	return D3D11TextRenderer( private_internals.pDevice, private_internals.pDeviceContext, font_name, font_size ).GetSize( viewport, text );
}
#endif
