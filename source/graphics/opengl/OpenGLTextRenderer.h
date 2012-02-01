#pragma once
#include <string>
#include <map>
#include "../BaseGraphicsDevice.h"
#include "../../geo/GeoFloat.h"

class OpenGLTextRenderer
{
public:
	OpenGLTextRenderer( const std::string& font_name, const unsigned int font_size );
	void Render( const unsigned int x, const unsigned int y, const std::string& text );
	void RenderBottomCenter( const GraphicsDeviceViewport& viewport, const std::string& text ); 
	GeoFloat2 GetSize(  const GraphicsDeviceViewport& viewport, const std::string& text  );
private:
	std::wstring font_name;
	unsigned int font_size;
};