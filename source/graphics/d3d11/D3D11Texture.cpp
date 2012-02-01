#ifdef _WIN32

#include "../inc.h"
#include "D3D11Texture.h"
#include <algorithm>
#include "D3D11GraphicsDevice.h"
#include "../../scenegraph/Sprite.h"

std::map<ID3D11Texture2D*, D3D11ShaderResource> D3D11Texture::shader_resources;

D3D11GraphicsDevice* D3D11Texture::pGraphicsDevice = nullptr;

D3D11Texture::D3D11Texture()
{
	valid = false;
}


///// consid
D3D11Texture::D3D11Texture( const std::string& image_filename )
{
	pDevice = pGraphicsDevice->GetInternals().pDevice;
	ID3D11Resource* pResource;
	valid = true;
	if( FAILED(D3DX11CreateTextureFromFile( pDevice, image_filename.c_str(), 0, 0, &pResource, 0 )) )
	{
		printf( "unable to create texture from file..\n" );
		valid = false;
	}
	pResource->QueryInterface( __uuidof( ID3D11Texture2D ), (LPVOID*)&pTex );
}

D3D11Texture::D3D11Texture( D3D11Texture a, D3D11Texture b )
{
	std::vector<D3D11Texture> textures;
	textures.push_back( a );
	textures.push_back( b );
	PrepareTextureArray( textures );
}

D3D11Texture::D3D11Texture( D3D11Texture a, D3D11Texture b, D3D11Texture c )
{
	std::vector<D3D11Texture> textures;
	textures.push_back( a );
	textures.push_back( b );
	textures.push_back( c );
	PrepareTextureArray( textures );
}

D3D11Texture::D3D11Texture( std::vector<D3D11Texture> textures )
{
	PrepareTextureArray( textures );	
}


void D3D11Texture::PrepareTextureArray( std::vector<D3D11Texture> textures )
{
	if( textures.empty() )
		throw "cannot create texture array with no textures";
	
	std::vector<D3D11_SUBRESOURCE_DATA> subresource_datas;
	for( unsigned int i = 0; i < textures.size(); i++ )
	{
		if( textures[i].GetWidth() != textures.front().GetWidth() || textures[i].GetHeight() != textures.front().GetHeight() )
		{
			//throw "attempt to create texture array with textures of differing dimensions!";
			textures[i] = textures[i].Clone( textures.front().GetWidth(), textures.front().GetHeight() );
		}
		
		D3D11_SUBRESOURCE_DATA subresource_data = textures[i].Read();
		subresource_datas.push_back( subresource_data );
	}
	
	Setup( textures.front().GetWidth(), textures.front().GetHeight(), D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DEFAULT, subresource_datas ); 
	for( unsigned i = 0; i < subresource_datas.size(); i++ )
		delete [] subresource_datas[i].pSysMem;
}

D3D11Texture D3D11Texture::Clone( const unsigned w, const unsigned int h )
{
	D3D11RenderTarget render_target = D3D11RenderTarget( w, h );
	pGraphicsDevice->SetRenderTarget( render_target );
	pGraphicsDevice->Clear( Color::Red() );
	pGraphicsDevice->SetViewport( w, h );
	pGraphicsDevice->GetStateManager().SetSpriteRendering();
#if RENDERER == RENDERER_D3D11
	Sprite( *this, GeoFloat2( -1, -1), GeoFloat2( 2, 2 ) ).Render();
#endif
	pGraphicsDevice->GetStateManager().SetDefaultFrontFaceRendering();
	pGraphicsDevice->SetDefaultRenderTarget();
	render_target.GetTexture().SaveToFile( "cloned_guy.dds" );
	return render_target.GrabTextureAndFree();
}

D3D11_SUBRESOURCE_DATA D3D11Texture::Read()
{
	   //D3D11_TEXTURE2D_DESC RTDESC;
	ID3D11Texture2D*                pStagingTexture = NULL;
	D3D11_TEXTURE2D_DESC RTDESC;
       ZeroMemory(&RTDESC, sizeof(D3D11_TEXTURE2D_DESC));
        RTDESC.Height = GetHeight();
        RTDESC.Width = GetWidth();
        RTDESC.MipLevels = 1;
        RTDESC.ArraySize = 1;
        RTDESC.SampleDesc.Count = 1;
        RTDESC.SampleDesc.Quality = 0;
		RTDESC.Format = texture_description.Format;
        RTDESC.Usage = D3D11_USAGE_STAGING;
        RTDESC.BindFlags = 0;
        RTDESC.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

       if( FAILED( pDevice->CreateTexture2D(&RTDESC, NULL, &pStagingTexture) ) )
	   {
		   printf( "could not create staging texture..\n" );
	   }

	   pDeviceContext->CopyResource( pStagingTexture, pTex ); 

	   if(FAILED(pDeviceContext->Map( pStagingTexture, 0, D3D11_MAP_READ, 0, &mapped_resource )))
	   {
			printf("failed to map textre\n");
	   }

	   D3D11_SUBRESOURCE_DATA subresource_data;
	   subresource_data.SysMemPitch = mapped_resource.RowPitch;
	   subresource_data.SysMemSlicePitch = 0;
	   subresource_data.pSysMem = new BYTE[ mapped_resource.RowPitch * GetHeight() ];
	   memcpy( (void*)subresource_data.pSysMem, mapped_resource.pData, mapped_resource.RowPitch * GetHeight() );
	  
	   pDeviceContext->Unmap( pStagingTexture, 0 );

	   return subresource_data;
}

D3D11Texture::D3D11Texture( const unsigned int width, const unsigned int height,  const unsigned int bind_flags, const D3D11_USAGE usage )
{
	Setup( width, height, bind_flags, usage );	
}

D3D11Texture::D3D11Texture( const unsigned int width, const unsigned int height, const Color& color )
{
	Setup( width, height, D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DYNAMIC );
	ClearColor( color );
}

D3D11Texture::D3D11Texture( const unsigned int width, const unsigned int height )
{
	Setup( width, height, D3D11_BIND_SHADER_RESOURCE, D3D11_USAGE_DYNAMIC );
}

bool D3D11Texture::IsFloatTexture()
{
	return true;
}

void D3D11Texture::Setup( const unsigned int width, const unsigned int height, const unsigned int bind_flags, const D3D11_USAGE usage  )
{
	Setup( width, height, bind_flags, usage, std::vector<D3D11_SUBRESOURCE_DATA>()  );
}

void D3D11Texture::Setup( const unsigned int width, const unsigned int height, const unsigned int bind_flags, const D3D11_USAGE usage, std::vector<D3D11_SUBRESOURCE_DATA> subresource_datas  )
{
	this->width = width;
	this->height = height;
	
	pDevice = pGraphicsDevice->GetInternals().pDevice;
	pDeviceContext = pGraphicsDevice->GetInternals().pDeviceContext;

	ZeroMemory( &texture_description , sizeof( texture_description ) );
	texture_description.Width = width;
	texture_description.Height = height;
	if( D3D11_USAGE_DYNAMIC == usage )
		texture_description.MipLevels = 1;
	else	
		texture_description.MipLevels = 1;
	texture_description.ArraySize = subresource_datas.empty() ? 1 : subresource_datas.size();
	texture_description.SampleDesc.Count = 1;
	texture_description.SampleDesc.Quality = 0;
	texture_description.Usage = usage;
	texture_description.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texture_description.BindFlags = bind_flags;
	
	if( D3D11_USAGE_DYNAMIC == usage )
		texture_description.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	else
		texture_description.CPUAccessFlags = 0;
	texture_description.MiscFlags = 0;

	bpp = BitsPerPixel( texture_description.Format ) / 8;
	
	bool failed = false;
	
	if( subresource_datas.empty() )
		failed = FAILED( pDevice->CreateTexture2D( &texture_description, 0, &pTex ) );
	else
		failed = FAILED( pDevice->CreateTexture2D( &texture_description, &subresource_datas[0], &pTex ) );

	if( failed )
		printf( "could not create texture with width %i and height %i!\n", width, height );
	else
	{
		printf("able to create the texxxxxx!\n" );
		valid = true;
	}
}


bool D3D11Texture::IsTextureArray()
{
	return texture_description.ArraySize > 1;
}

ID3D11Texture2D* D3D11Texture::GetTex()
{
	return pTex;
}

DXGI_FORMAT D3D11Texture::GetFormat()
{
	return texture_description.Format;
}

D3D11ShaderResource& D3D11Texture::GetShaderResource()
{
	if( !shader_resource.IsCreated() )
	{
		if( shader_resources.find( pTex ) == shader_resources.end() )
		{	
			printf( "recreating it....\n" );
			shader_resource = D3D11ShaderResource( pDevice, pTex );
			shader_resources[pTex] = shader_resource;
		}
		else
			shader_resource = shader_resources[pTex];
	}
	return shader_resource;
}

void* D3D11Texture::Map( unsigned int* pPitch )
{
	pTex->GetDesc( &texture_description );
	if(FAILED(pDeviceContext->Map( pTex, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource )))
	{
		printf("failed to map textre\n");
		return 0;
	}
	
	if( pPitch )
		*pPitch = mapped_resource.RowPitch;

	return mapped_resource.pData;
}

void D3D11Texture::Unmap()
{
	pDeviceContext->Unmap( pTex, 0 );
}

bool D3D11Texture::SaveToFile( const std::string& filename, const bool save_only_once )
{
	if( save_only_once && saved )
		return true;
	saved = true;
	
	std::string extension = filename.substr(filename.find_last_of(".")+1);
	std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
	D3DX11_IMAGE_FILE_FORMAT file_format;
	if( "bmp" == extension )
		file_format = D3DX11_IFF_BMP;
	else if( "jpg" == extension )
		file_format = D3DX11_IFF_JPG;
	else if( "png" == extension )
		file_format = D3DX11_IFF_PNG;
	else if( "dds" == extension )
		file_format = D3DX11_IFF_DDS;
	else if( "tiff" == extension )
		file_format = D3DX11_IFF_TIFF;
	else if( "gif" == extension )
		file_format = D3DX11_IFF_GIF;
	else if( "wmp" == extension )
		file_format = D3DX11_IFF_WMP;
	else
		return false;
	
	if( FAILED( D3DX11SaveTextureToFile( pDeviceContext, pTex, file_format, filename.c_str() ) ) ) 
	{
		printf( "could not save texture to file..\n" );
		return false;
	}

	return true;
}

unsigned int D3D11Texture::BitsPerPixel( DXGI_FORMAT fmt )
{
    switch( fmt )
    {
    case DXGI_FORMAT_R32G32B32A32_TYPELESS:
    case DXGI_FORMAT_R32G32B32A32_FLOAT:
    case DXGI_FORMAT_R32G32B32A32_UINT:
    case DXGI_FORMAT_R32G32B32A32_SINT:
        return 128;

    case DXGI_FORMAT_R32G32B32_TYPELESS:
    case DXGI_FORMAT_R32G32B32_FLOAT:
    case DXGI_FORMAT_R32G32B32_UINT:
    case DXGI_FORMAT_R32G32B32_SINT:
        return 96;

    case DXGI_FORMAT_R16G16B16A16_TYPELESS:
    case DXGI_FORMAT_R16G16B16A16_FLOAT:
    case DXGI_FORMAT_R16G16B16A16_UNORM:
    case DXGI_FORMAT_R16G16B16A16_UINT:
    case DXGI_FORMAT_R16G16B16A16_SNORM:
    case DXGI_FORMAT_R16G16B16A16_SINT:
    case DXGI_FORMAT_R32G32_TYPELESS:
    case DXGI_FORMAT_R32G32_FLOAT:
    case DXGI_FORMAT_R32G32_UINT:
    case DXGI_FORMAT_R32G32_SINT:
    case DXGI_FORMAT_R32G8X24_TYPELESS:
    case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
    case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
    case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
        return 64;

    case DXGI_FORMAT_R10G10B10A2_TYPELESS:
    case DXGI_FORMAT_R10G10B10A2_UNORM:
    case DXGI_FORMAT_R10G10B10A2_UINT:
    case DXGI_FORMAT_R11G11B10_FLOAT:
    case DXGI_FORMAT_R8G8B8A8_TYPELESS:
    case DXGI_FORMAT_R8G8B8A8_UNORM:
    case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
    case DXGI_FORMAT_R8G8B8A8_UINT:
    case DXGI_FORMAT_R8G8B8A8_SNORM:
    case DXGI_FORMAT_R8G8B8A8_SINT:
    case DXGI_FORMAT_R16G16_TYPELESS:
    case DXGI_FORMAT_R16G16_FLOAT:
    case DXGI_FORMAT_R16G16_UNORM:
    case DXGI_FORMAT_R16G16_UINT:
    case DXGI_FORMAT_R16G16_SNORM:
    case DXGI_FORMAT_R16G16_SINT:
    case DXGI_FORMAT_R32_TYPELESS:
    case DXGI_FORMAT_D32_FLOAT:
    case DXGI_FORMAT_R32_FLOAT:
    case DXGI_FORMAT_R32_UINT:
    case DXGI_FORMAT_R32_SINT:
    case DXGI_FORMAT_R24G8_TYPELESS:
    case DXGI_FORMAT_D24_UNORM_S8_UINT:
    case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
    case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
    case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
    case DXGI_FORMAT_R8G8_B8G8_UNORM:
    case DXGI_FORMAT_G8R8_G8B8_UNORM:
    case DXGI_FORMAT_B8G8R8A8_UNORM:
    case DXGI_FORMAT_B8G8R8X8_UNORM:
    case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
    case DXGI_FORMAT_B8G8R8A8_TYPELESS:
    case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
    case DXGI_FORMAT_B8G8R8X8_TYPELESS:
    case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
        return 32;

    case DXGI_FORMAT_R8G8_TYPELESS:
    case DXGI_FORMAT_R8G8_UNORM:
    case DXGI_FORMAT_R8G8_UINT:
    case DXGI_FORMAT_R8G8_SNORM:
    case DXGI_FORMAT_R8G8_SINT:
    case DXGI_FORMAT_R16_TYPELESS:
    case DXGI_FORMAT_R16_FLOAT:
    case DXGI_FORMAT_D16_UNORM:
    case DXGI_FORMAT_R16_UNORM:
    case DXGI_FORMAT_R16_UINT:
    case DXGI_FORMAT_R16_SNORM:
    case DXGI_FORMAT_R16_SINT:
    case DXGI_FORMAT_B5G6R5_UNORM:
    case DXGI_FORMAT_B5G5R5A1_UNORM:
        return 16;

    case DXGI_FORMAT_R8_TYPELESS:
    case DXGI_FORMAT_R8_UNORM:
    case DXGI_FORMAT_R8_UINT:
    case DXGI_FORMAT_R8_SNORM:
    case DXGI_FORMAT_R8_SINT:
    case DXGI_FORMAT_A8_UNORM:
        return 8;

    case DXGI_FORMAT_R1_UNORM:
        return 1;

    case DXGI_FORMAT_BC1_TYPELESS:
    case DXGI_FORMAT_BC1_UNORM:
    case DXGI_FORMAT_BC1_UNORM_SRGB:
        return 4;

    case DXGI_FORMAT_BC2_TYPELESS:
    case DXGI_FORMAT_BC2_UNORM:
    case DXGI_FORMAT_BC2_UNORM_SRGB:
    case DXGI_FORMAT_BC3_TYPELESS:
    case DXGI_FORMAT_BC3_UNORM:
    case DXGI_FORMAT_BC3_UNORM_SRGB:
    case DXGI_FORMAT_BC4_TYPELESS:
    case DXGI_FORMAT_BC4_UNORM:
    case DXGI_FORMAT_BC4_SNORM:
    case DXGI_FORMAT_BC5_TYPELESS:
    case DXGI_FORMAT_BC5_UNORM:
    case DXGI_FORMAT_BC5_SNORM:
    case DXGI_FORMAT_BC6H_TYPELESS:
    case DXGI_FORMAT_BC6H_UF16:
    case DXGI_FORMAT_BC6H_SF16:
    case DXGI_FORMAT_BC7_TYPELESS:
    case DXGI_FORMAT_BC7_UNORM:
    case DXGI_FORMAT_BC7_UNORM_SRGB:
        return 8;

    default:
       // assert( FALSE ); // unhandled format
        return 0;
    }
}

#endif
