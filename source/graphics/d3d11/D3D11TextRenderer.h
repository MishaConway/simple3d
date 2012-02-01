#pragma once
#ifdef _WIN32

#include "FW1FontWrapper.h"
#include <D3DCompiler.h>
#include <xnamath.h>
#include <string>
#include <map>
#include "../BaseGraphicsDevice.h"
#include "../../geo/GeoFloat.h"

class D3D11TextRenderer
{
public:
	D3D11TextRenderer( ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& font_name, const unsigned int font_size );
	void Render( const unsigned int x, const unsigned int y, const std::string& text );
	void RenderBottomCenter( const GraphicsDeviceViewport& viewport, const std::string& text ); 
	GeoFloat2 GetSize(  const GraphicsDeviceViewport& viewport, const std::string& text  );
private:
	IFW1FontWrapper* pFontWrapper;
	ID3D11DeviceContext* pDeviceContext;
	std::wstring font_name;
	unsigned int font_size;
	static std::map< std::string, IFW1FontWrapper* > font_wrappers;
	static IFW1Factory* pFW1Factory;
};
#endif