#include "OpenGLRenderTarget.h"
#include "OpenGLGraphicsDevice.h"
#include "../../scenegraph/Sprite.h"

OpenGLGraphicsDevice* OpenGLRenderTarget::pGraphicsDevice;

OpenGLRenderTarget::OpenGLRenderTarget()
{
	valid = false;
}
	
OpenGLRenderTarget::OpenGLRenderTarget( const unsigned int width, const unsigned int height )
{
	tex = OpenGLTexture( width, height, OpenGLTextureUsage::render_target ); 
	tex.SetTiling( 1, -1 );

	//generate and bind an opengl frame buffer object for this texture
	glGenFramebuffersEXT( 1, &fbo_id );
	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, fbo_id );

	//attach the texture object to this framebuffer object
	glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, tex.GetOpenGLTextureId(), 0 );

	glGenRenderbuffersEXT(1, &depth_buffer_id);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depth_buffer_id);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, width, height);
	
	//-------------------------
	//Attach depth buffer to FBO
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depth_buffer_id);

	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT); 
	switch (status) 
	{	
		case GL_FRAMEBUFFER_COMPLETE_EXT: 
			printf("framebuffer complete\n"); 
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED_EXT: 
			printf("framebuffer unsupported\n"); 
			break;
		default: 
			printf("FORGET IT!\n");
			break;
	} 

	//now that a framebuffer object has been created and attached to this texture, we can unbind it
	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
}

void OpenGLRenderTarget::Free()
{

}
	
OpenGLTexture OpenGLRenderTarget::GrabTextureAndFree()
{
	return GetTexture();
}

OpenGLTexture& OpenGLRenderTarget::GetTexture()
{
	return tex;
}

bool OpenGLRenderTarget::HorizontalBlur( OpenGLRenderTarget render_target_written )
{
	return Blur( render_target_written, "PostProcessHorizontalBlur" );
}

bool OpenGLRenderTarget::VerticalBlur( OpenGLRenderTarget render_target_written )
{
	return Blur( render_target_written, "PostProcessVerticalBlur" );
}

bool OpenGLRenderTarget::Blur( OpenGLRenderTarget pRenderTargetWritten, const std::string& blur_technique )
{
	Effect::GetCurrentEffect().SetFloatArray( "BlurWeights", ComputeGaussianKernel( 9, 400.5f  ) );
	pGraphicsDevice->SetRenderTarget( pRenderTargetWritten );
#if RENDERER == RENDERER_OPENGL
	Sprite( GetTexture(), GeoFloat2( -1, -1), GeoFloat2( 2, 2 ), BlendType::NONE, blur_technique ).Render();
#endif
	return true;
}


