#pragma once

#include "OpenGLTexture.h"
#include "OpenGLEffect.h"
#include "../BaseRenderTarget.h"
#include <vector>

#ifdef GL_ES_VERSION_2_0
#define DEFAULT_DEPTH_BUFFER_FORMAT GL_DEPTH_COMPONENT16
#else
#define DEFAULT_DEPTH_BUFFER_FORMAT GL_DEPTH_COMPONENT24
#endif


//class Effect;
class OpenGLRenderTarget : public BaseRenderTarget
{
friend class OpenGLGraphicsDevice;
public:
	OpenGLRenderTarget();
	OpenGLRenderTarget( const unsigned int width, const unsigned int height );
	OpenGLTexture& GetTexture();
	bool HorizontalBlur( OpenGLRenderTarget pRenderTargetWritten );
	bool VerticalBlur( OpenGLRenderTarget pRenderTargetWritten );
	
	void Free();
	OpenGLTexture GrabTextureAndFree();
private:
	bool Blur( OpenGLRenderTarget pRenderTargetWritten, const std::string& blur_technique );
private:
	OpenGLTexture tex;
	GLuint fbo_id, depth_buffer_id;
	static OpenGLGraphicsDevice* pGraphicsDevice;
};