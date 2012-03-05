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
	glGenFramebuffers( 1, &fbo_id );
	glBindFramebuffer( GL_FRAMEBUFFER, fbo_id );

	//attach the texture object to this framebuffer object
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex.GetOpenGLTextureId(), 0 );

	glGenRenderbuffers(1, &depth_buffer_id);
	glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer_id);
	glRenderbufferStorage(GL_RENDERBUFFER, DEFAULT_DEPTH_BUFFER_FORMAT, width, height);
	
	//-------------------------
	//Attach depth buffer to FBO
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer_id);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER); 
	switch (status) 
	{	
		case GL_FRAMEBUFFER_COMPLETE: 
			printf("framebuffer complete\n"); 
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED: 
			printf("framebuffer unsupported\n"); 
			break;
		default: 
			printf("FORGET IT!\n");
			break;
	} 

	//now that a framebuffer object has been created and attached to this texture, we can unbind it
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
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
	Sprite( GetTexture(), GeoFloat2( 0, 0), GeoFloat2( 2, 2 ), BlendType::NONE, blur_technique ).Render();
#endif
	return true;
}


