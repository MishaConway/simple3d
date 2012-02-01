#pragma once
#include "Quad.h"

class Sprite : public Quad
{
public:
	Sprite();
	Sprite( Texture t );
	Sprite( Texture t, const GeoFloat2& p, const GeoFloat2& s );
	Sprite( Texture t, const GeoFloat2& p, const GeoFloat2& s, const BlendType::GRAPHICS_DEVICE_STATE_MANAGER_BLEND_TYPE blend_type );
	Sprite( Texture t, const GeoFloat2& p, const GeoFloat2& s, const BlendType::GRAPHICS_DEVICE_STATE_MANAGER_BLEND_TYPE blend_type, const std::string& technique );
	void SetPosition( const GeoFloat2& p );
	void SetSize( const GeoFloat2& s );
	void SetWidth( const float w );
	void SetHeight( const float h );
	GeoFloat2 GetSize();
	GeoFloat2 GetPosition();
	virtual std::string GetDefaultTechnique();
	virtual bool Render();
protected:
	GeoFloat2 position, size;
};

