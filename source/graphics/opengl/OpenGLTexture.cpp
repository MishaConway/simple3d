#include "OpenGLTexture.h"
#include <algorithm>
#include "OpenGLGraphicsDevice.h"
#include "../../scenegraph/Sprite.h"

OpenGLGraphicsDevice* OpenGLTexture::pGraphicsDevice = nullptr;

OpenGLTexture::OpenGLTexture()
{
	valid = false;
}

///// consid
OpenGLTexture::OpenGLTexture( const std::string& image_filename )
{
	SetTiling( 1, 1 );
	SetOffsets( 0, 0 );
	glEnable(GL_TEXTURE_2D);
	texture_id = SOIL_load_OGL_texture( image_filename.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,  SOIL_FLAG_MIPMAPS );
	if( !texture_id )
	{
		printf( "soil failed to load %s\n", image_filename.c_str() );
		printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
	}
	glEnable(GL_TEXTURE_2D);
	//store the dimensions of this texture
	glBindTexture( GL_TEXTURE_2D, texture_id  ); 

	glTexParameterf(GL_TEXTURE_2D,GL_GENERATE_MIPMAP, GL_TRUE);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MAG_FILTER,  GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//glGenerateMipmap( GL_TEXTURE_2D );
	//glGenerateMipmapEXT( GL_TEXTURE_2D );
	int w, h;
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH,	&w  );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT,	&h );
	width = (unsigned int) w;
	height = (unsigned int) h;
	
	//resolve the number of color components in this texture
	GLint RedSize, GreenSize, BlueSize, AlphaSize, LuminanceSize, IntensitySize, NumColorComponents = 0;
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_RED_SIZE      , &RedSize );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_GREEN_SIZE    , &GreenSize );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_BLUE_SIZE     , &BlueSize);
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_ALPHA_SIZE    , &AlphaSize );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_LUMINANCE_SIZE, &LuminanceSize );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_INTENSITY_SIZE, &IntensitySize );
	if( RedSize )
		NumColorComponents++;
	if( GreenSize )
		NumColorComponents++;
	if( BlueSize )
		NumColorComponents++;
	if( AlphaSize )
		NumColorComponents++;
	if( LuminanceSize )
		NumColorComponents++;
	if( IntensitySize )
		NumColorComponents++;
		//(*ppTexture)->numColorComponents = NumColorComponents;
	
	//resolve the color component stride
		GLint ColorComponentStrideInBits = -1;
		glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_RED_SIZE, &ColorComponentStrideInBits );
		//(*ppTexture)->colorComponentStride = ColorComponentStrideInBits / 8;
		//if( 0 == (*ppTexture)->colorComponentStride )
		{
		//	printf( "what the hell?" );
		//	(*ppTexture)->colorComponentStride = 1;
		}
		
		//HACK: component type is hard coded
		//(*ppTexture)->componentType = GL_UNSIGNED_BYTE;
	//glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, (GLint*) &((*ppTexture)->format ) );

		valid = true;
}


OpenGLTexture::OpenGLTexture( OpenGLTexture a, OpenGLTexture b )
{
	std::vector<OpenGLTexture> textures;
	textures.push_back( a );
	textures.push_back( b );
	PrepareTextureArray( textures );
}

OpenGLTexture::OpenGLTexture( OpenGLTexture a, OpenGLTexture b, OpenGLTexture c )
{
	std::vector<OpenGLTexture> textures;
	textures.push_back( a );
	textures.push_back( b );
	textures.push_back( c );
	PrepareTextureArray( textures );
}

OpenGLTexture::OpenGLTexture( std::vector<OpenGLTexture> textures )
{
	PrepareTextureArray( textures );	
}


void OpenGLTexture::PrepareTextureArray( std::vector<OpenGLTexture> textures )
{
}

OpenGLTexture OpenGLTexture::Clone( const unsigned w, const unsigned int h )
{
	OpenGLRenderTarget render_target = OpenGLRenderTarget( w, h );
	pGraphicsDevice->SetRenderTarget( render_target );
	pGraphicsDevice->Clear( Color::Red() );
	pGraphicsDevice->SetViewport( w, h );
	pGraphicsDevice->GetStateManager().SetSpriteRendering();
#if RENDERER == RENDERER_OPENGL
	Sprite( *this, GeoFloat2( -1, -1), GeoFloat2( 2, 2 ) ).Render();
#endif
	pGraphicsDevice->GetStateManager().SetDefaultFrontFaceRendering();
	pGraphicsDevice->SetDefaultRenderTarget();
	render_target.GetTexture().SaveToFile( "cloned_guy.dds" );
	return render_target.GrabTextureAndFree(); 
	return OpenGLTexture();
}

/*
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
*/

OpenGLTexture::OpenGLTexture( const unsigned int width, const unsigned int height,  const OpenGLTextureUsage::OPENGL_TEXTURE_USAGE usage )
{
	Setup( width, height, usage );	
}

OpenGLTexture::OpenGLTexture( const unsigned int width, const unsigned int height, const Color& color )
{
	Setup( width, height, OpenGLTextureUsage::shader_resource );
	ClearColor( color );
}

OpenGLTexture::OpenGLTexture( const unsigned int width, const unsigned int height )
{
	Setup( width, height, OpenGLTextureUsage::shader_resource );
}

bool OpenGLTexture::IsFloatTexture()
{
	return component_type == GL_FLOAT;
}

void OpenGLTexture::Setup( const unsigned int width, const unsigned int height, const OpenGLTextureUsage::OPENGL_TEXTURE_USAGE usage  )
{
	internal_format = GL_RGBA32F;
	//internal_format = GL_RGBA;
	//internal_format = GL_RGBA8;
	external_format = GL_RGBA;
	component_type = GL_FLOAT;

	if( GL_RGBA32F_ARB == internal_format )
	{
		component_type = GL_FLOAT;
		bpp = sizeof(float)*4;
	}
	else
	{
		component_type = GL_UNSIGNED_BYTE;
		bpp = 4;
	}
	
	this->width = width;
	this->height = height;	
	format = GL_RGBA;
	
	glEnable( GL_TEXTURE_2D );
	glGenTextures(1, &texture_id );
	glGenBuffers( 1, &pbo_id  );

	//bind our created opengl texture object so we may define its parameters
	glBindTexture( GL_TEXTURE_2D, texture_id  );	
	
	if( OpenGLTextureUsage::render_target == usage  )
	{
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	//Upload the textel buffer using Pixel Buffer Objects
	glBindBufferARB( GL_PIXEL_UNPACK_BUFFER_ARB, pbo_id );
	glBufferDataARB( GL_PIXEL_UNPACK_BUFFER_ARB, width*height*bpp, 0, GL_STREAM_DRAW );
	glTexImage2D( GL_TEXTURE_2D, 0, internal_format, width, height, 0, external_format, component_type, (const GLubyte*) 0 );
	glBindBufferARB( GL_PIXEL_UNPACK_BUFFER_ARB, 0 );

	valid = true;
}

bool OpenGLTexture::IsTextureArray()
{
	return false; //texture_description.ArraySize > 1;
}

GLint OpenGLTexture::GetOpenGLTextureId()
{
	return valid ? texture_id : 0;
}

GLenum OpenGLTexture::GetFormat()
{
	return format;
}

void* OpenGLTexture::Map( unsigned int* pPitch )
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture( GL_TEXTURE_2D, texture_id  ); 
	
	pMappedData = nullptr;
	const unsigned int buffer_size = width*height*bpp;
	if( pGraphicsDevice->GetCapabilities().SupportsPixelBufferObject() )
	{
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, pbo_id );

		//we can quickly invalidate the data stored on the GPU by nulling it
		//before mapping the buffer to the client side. This allows the driver to avoid stalls due to 
		//attempting to maintain a copy of the old textel buffer.
		glBufferData(GL_PIXEL_UNPACK_BUFFER_ARB, buffer_size, (const GLubyte*) 0, GL_DYNAMIC_DRAW);

		//Map our Pixel Buffer Object into client side memory
		pMappedData = glMapBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, GL_WRITE_ONLY);

		//Unbind our Pixel Buffer Object
		glBindBufferARB( GL_PIXEL_UNPACK_BUFFER_ARB, 0 );
	}
	else
		pMappedData = new unsigned char[buffer_size];

	if( pPitch )
		*pPitch = width * bpp;
	return pMappedData;
}

void OpenGLTexture::Unmap()
{
	//Bind our Pixel Buffer Object and attempt to unmap it...
	glBindTexture( GL_TEXTURE_2D, texture_id  ); 
	if( pGraphicsDevice->GetCapabilities().SupportsPixelBufferObject() )
	{
		glBindBufferARB( GL_PIXEL_UNPACK_BUFFER_ARB, pbo_id ); 			
		glUnmapBufferARB( GL_PIXEL_UNPACK_BUFFER_ARB );
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, external_format, component_type, 0 );

		//Unbind our Pixel Buffer Object
		glBindBufferARB( GL_PIXEL_UNPACK_BUFFER_ARB, 0 );
	}
	else
	{
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, external_format, component_type, pMappedData );
		delete [] pMappedData;
		pMappedData = nullptr;
	}
	glGenerateMipmap( GL_TEXTURE_2D );
}

bool OpenGLTexture::SaveToFile( const std::string& filename, const bool save_only_once )
{
	if( save_only_once && saved )
		return true;
	saved = true;
	
	std::string extension = filename.substr(filename.find_last_of(".")+1);
	std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
	int file_format;
	if( "bmp" == extension )
		file_format = SOIL_SAVE_TYPE_BMP;
	else if( "dds" == extension )
		file_format = SOIL_SAVE_TYPE_DDS;
	else
		return false;

	GLint is_compressed = GL_FALSE;
	GLint compressed_size = 0;
	GLint compressed_format = 0;
	glBindTexture( GL_TEXTURE_2D, texture_id  );
	glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED_ARB, &is_compressed );
	if( is_compressed )
	{
		glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &compressed_size );
		glGetTexLevelParameteriv( GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &compressed_format );
		printf( "sorry can't save compressed textures yet...\n" );
	}
	else
	{
		unsigned char* pixels = new unsigned char[ width*height*bpp ];
		glGetTexImage( GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );
	
		if( !SOIL_save_image( filename.c_str(), file_format, width, height, 4, pixels ) )
			printf( "SOIL_save_image failed!\n" );
		delete [] pixels;
	}
	
	return true;
}

