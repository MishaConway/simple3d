#ifdef _WIN32
#include "D3D11TextRenderer.h"

IFW1Factory* D3D11TextRenderer::pFW1Factory = nullptr;
std::map< std::string, IFW1FontWrapper* > D3D11TextRenderer::font_wrappers;

D3D11TextRenderer::D3D11TextRenderer( ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, const std::string& font_name, const unsigned int font_size )
{
	this->pDeviceContext = pDeviceContext;
	this->font_size = font_size;
	this->font_name = std::wstring(font_name.begin(), font_name.end());
    this->font_name.assign(font_name.begin(), font_name.end());

	if( !pFW1Factory )	
		if( FAILED( FW1CreateFactory(FW1_VERSION, &pFW1Factory) ) )
			printf( "could not create factory\n" );

	if( font_wrappers.count(font_name) )
		pFontWrapper = font_wrappers[font_name];	
	else
	{
		if( FAILED( pFW1Factory->CreateFontWrapper(pDevice, L"Arial", &pFontWrapper) ) )
			printf( "could not create font wrappr\n" );
		font_wrappers[font_name] = pFontWrapper;
	}
}

GeoFloat2 D3D11TextRenderer::GetSize(  const GraphicsDeviceViewport& viewport, const std::string& text  )
{
	FW1_RECTF clip_rect;
	clip_rect.Left = clip_rect.Top = 0;
	clip_rect.Bottom = viewport.Height;
	clip_rect.Right = viewport.Width;

	std::wstring w(text.begin(), text.end());
    w.assign(text.begin(), text.end());

	FW1_RECTF measured_rect = pFontWrapper->MeasureString( w.c_str(), font_name.c_str(), (float)font_size, &clip_rect, FW1_RESTORESTATE | FW1_NOGEOMETRYSHADER | FW1_CENTER | FW1_BOTTOM | FW1_ALIASED ); 

	return GeoFloat2( abs( measured_rect.Right - measured_rect.Left), abs( measured_rect.Top - measured_rect.Bottom ) );
}

void D3D11TextRenderer::RenderBottomCenter( const GraphicsDeviceViewport& viewport, const std::string& text )
{
	FW1_RECTF clip_rect;
	clip_rect.Left = clip_rect.Top = 0;
	clip_rect.Bottom = viewport.Height;
	clip_rect.Right = viewport.Width;

	std::wstring w(text.begin(), text.end());
    w.assign(text.begin(), text.end());

	pFontWrapper->DrawString( pDeviceContext, w.c_str(), font_name.c_str(), (float)font_size, &clip_rect, 0xFFF5F5DC, &clip_rect, 0, FW1_RESTORESTATE | FW1_NOGEOMETRYSHADER | FW1_CENTER | FW1_BOTTOM | FW1_ALIASED ); 
}

void D3D11TextRenderer::Render( const unsigned int x, const unsigned int y, const std::string& text )
{
	std::wstring w(text.begin(), text.end());
    w.assign(text.begin(), text.end());
	pFontWrapper->DrawString(pDeviceContext, w.c_str(), (float)font_size, (float)x, (float)y, 0xFF000000, FW1_RESTORESTATE | FW1_NOGEOMETRYSHADER | FW1_RIGHT );
}
#endif