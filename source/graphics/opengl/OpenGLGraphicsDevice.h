#pragma once

#include "include_opengl.h"
#include <string>
#include "../BaseGraphicsDevice.h"
#include "OpenGLGraphicsDeviceStateManager.h"
#include "OpenGLCapabilities.h"
#include "OpenGLVertexBuffer.h"
#include "OpenGLDynamicVertexBuffer.h"
#include "OpenGLTexture.h"
#include "OpenGLRenderTarget.h"
#include "OpenGLTextRenderer.h"

class OpenGLGraphicsDevice : public BaseGraphicsDevice
{
public:
	OpenGLGraphicsDevice();
	std::string GetRendererType();

	bool Initialize( HWND hWnd, const unsigned int width, const unsigned int height, const bool debug );

	bool Draw( const unsigned int num_vertices );

	virtual void SetViewport( const unsigned int width, const unsigned int height );
	
	OpenGLGraphicsDeviceStateManager& GetStateManager();
	OpenGLCapabilities& GetCapabilities();
	
	virtual void Clear( Color color );
	bool SwapBackBuffer();

	void    SetVertexBuffer( OpenGLVertexBuffer& pVertexBuffer );
	void    SetVertexBuffer( OpenGLDynamicVertexBuffer& pVertexBuffer );

	void SetDefaultRenderTarget();
	void SetRenderTarget( OpenGLRenderTarget& render_target );
	void SetRenderTarget(OpenGLRenderTarget& render_target, const Color& clear_color );
	
	void RenderText( const unsigned int x, const unsigned int y, const unsigned int font_size, const std::string& font_name, const std::string& text );
	void RenderTextToBottomCenter( const unsigned int font_size, const std::string& font_name, const std::string& text );
	GeoFloat2 GetTextSize( const unsigned int font_size, const std::string& font_name, const std::string& text );
private:
	OpenGLGraphicsDeviceStateManager graphics_device_state_manager;
	OpenGLCapabilities capabilities;
	OpenGLVertexBuffer* pBoundVertexBuffer;
	HDC hdc;
};