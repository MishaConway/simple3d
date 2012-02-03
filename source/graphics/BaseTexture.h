#pragma once

#include "../std/functional.h"
#include "../geo/GeoFloat.h"
#include "Color.h"
#include "../interfaces/IValidatable.h"

class BaseTexture : public IValidatable
{
public:
	BaseTexture();

	virtual unsigned int GetWidth();
	virtual unsigned int GetHeight();
	virtual unsigned int GetBytesPerPixel();
	//virtual bool IsRGBAOrder() = 0;
	//virtual bool IsBGRAOrder() = 0;
	//virtual unsigned int GetNumComponents() = 0;
	virtual void SetTiling( const float tiling );
	virtual void SetTiling( const float horizontal_tiling, const float vertical_tiling );
	virtual void SetOffsets( const float horizontal_offset, const float vertical_offset );
	virtual float GetHorizontalTiling();
	virtual float GetVerticalTiling();
	virtual float GetHorizontalOffset();
	virtual float GetVerticalOffset();
	virtual bool IsFloatTexture() = 0;

#ifdef _WIN32	
	virtual bool SetData( std::function<void(const unsigned int x, const unsigned int y, float* pRed, float* pGreen, float* pBlue, float* pAlpha)> write_pixel );
#endif
	virtual bool ClearColor( Color color, const bool preserve_alpha = false );
protected:
	void* Map();
	virtual void* Map( unsigned int* pPitch ) = 0;
	virtual void Unmap() = 0;

protected:
	unsigned int width, height, bpp;
	float horizontal_tiling_scale, vertical_tiling_scale;
	float horizontal_offset, vertical_offset;
	bool saved;
};