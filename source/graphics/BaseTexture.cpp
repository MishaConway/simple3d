#include "BaseTexture.h"

BaseTexture::BaseTexture()
{
	width = height = bpp = 0;
	SetTiling( 1, 1 );
	SetOffsets( 0, 0 );
	saved = false;
}

void BaseTexture::SetTiling( const float tiling )
{
	SetTiling( tiling, tiling );
}

void BaseTexture::SetTiling( const float horizontal_tiling, const float vertical_tiling )
{
	this->horizontal_tiling_scale = horizontal_tiling;
	this->vertical_tiling_scale = vertical_tiling;
}

void BaseTexture::SetOffsets( const float horizontal_offset, const float vertical_offset )
{
	this->horizontal_offset = horizontal_offset;
	this->vertical_offset = vertical_offset;
}

unsigned int BaseTexture::GetWidth()
{
	return width;
}

unsigned int BaseTexture::GetHeight()
{
	return height;
}

unsigned int BaseTexture::GetBytesPerPixel()
{
	return bpp;
}

float BaseTexture::GetHorizontalOffset()
{
	return horizontal_offset;
}

float BaseTexture::GetVerticalOffset()
{
	return vertical_offset;
}

float BaseTexture::GetHorizontalTiling()
{
	return horizontal_tiling_scale;
}

float BaseTexture::GetVerticalTiling()
{
	return vertical_tiling_scale;
}

unsigned char* BaseTexture::Map()
{
	return Map( nullptr );
}

#ifdef _WIN32	
bool BaseTexture::SetData( std::function<void(const unsigned int x, const unsigned int y, float* pRed, float* pGreen, float* pBlue, float* pAlpha)> write_pixel )
#endif
#if defined(__APPLE__) || defined(__APPLE_CC__)  
bool BaseTexture::SetData( void(^write_pixel)(const unsigned int x, const unsigned int y, float* pRed, float* pGreen, float* pBlue, float* pAlpha) )
#endif
{
	unsigned int pitch;
	unsigned char* pMappedData = Map( &pitch );	
	for( unsigned int row = 0; row < GetHeight(); row++ )
		for( unsigned int i = 0; i < GetWidth(); i++ )
			if( IsFloatTexture() )
			{
				write_pixel( i, row, (float*)(pMappedData + row*pitch + i*bpp),
					 (float*)(pMappedData + row*pitch + i*bpp + sizeof(float)),
					 (float*)(pMappedData + row*pitch + i*bpp + sizeof(float)*2),
					 (float*)(pMappedData + row*pitch + i*bpp + sizeof(float)*3) );
			}
			else
			{
				float red, green, blue, alpha;
				write_pixel( i, row, &red, &green, &blue, &alpha );
				*(pMappedData + row*pitch + i*bpp) = (unsigned char)(red * 255.0f);
				*(pMappedData + row*pitch + i*bpp + 1) = (unsigned char)(green * 255.0f);
				*(pMappedData + row*pitch + i*bpp + 2) = (unsigned char)(blue * 255.0f);
				*(pMappedData + row*pitch + i*bpp + 3) = (unsigned char)(alpha * 255.0f);
			}
	Unmap();
	return true;
}


bool BaseTexture::ClearColor( Color color, const bool preserve_alpha )
{
	float red = color.GetNormalizedRed();
	float green = color.GetNormalizedGreen();
	float blue = color.GetNormalizedBlue();
	float alpha = color.GetNormalizedAlpha();
#ifdef _WIN32	
	return SetData( [this, &red, &green, &blue, &alpha, &preserve_alpha](const unsigned int x, const unsigned int y, float* pRed, float* pGreen, float* pBlue, float* pAlpha)
#endif
#if defined(__APPLE__) || defined(__APPLE_CC__)  
    return SetData( ^(const unsigned int x, const unsigned int y, float* pRed, float* pGreen, float* pBlue, float* pAlpha)               
#endif
    { 
		*pRed = red;
		*pGreen = green;
		*pBlue = blue;
		if( !preserve_alpha )
			*pAlpha = alpha; 
	});
}
