#include "OpenGLGraphicsDevice.h"
#include "../../std/nullptr.h"
#if defined(__APPLE__) || defined(__APPLE_CC__) 
#include "../../system/CocoaBindings.h"
#endif


OpenGLGraphicsDevice::OpenGLGraphicsDevice()
{
}

std::string OpenGLGraphicsDevice::GetRendererType()
{
	return "OpenGL";
}

bool OpenGLGraphicsDevice::Initialize( HWND hWnd, const unsigned int width, const unsigned int height, const bool debug )
{	  
	if( initialized )
		return true;
    
    #if defined(__APPLE__) || defined(__APPLE_CC__) 
    SetCocoaBindings();
    #endif
    

#ifdef _WIN32
	hdc = GetDC(hWnd); // Get the device context for our window  
  
	PIXELFORMATDESCRIPTOR pfd; // Create a new PIXELFORMATDESCRIPTOR (PFD)  
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR)); // Clear our  PFD  
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR); // Set the size of the PFD to the size of the class  
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW; // Enable double buffering, opengl support and drawing to a window  
	pfd.iPixelType = PFD_TYPE_RGBA; // Set our application to use RGBA pixels  
	pfd.cColorBits = 32; // Give us 32 bits of color information (the higher, the more colors)  
	pfd.cDepthBits = 32; // Give us 32 bits of depth information (the higher, the more depth levels)  
	pfd.iLayerType = PFD_MAIN_PLANE; // Set the layer of the PFD  

	int nPixelFormat = ChoosePixelFormat(hdc, &pfd); // Check if our PFD is valid and get a pixel format back  
	if (nPixelFormat == 0) // If it fails  
		return false;  
  
	if( !SetPixelFormat(hdc, nPixelFormat, &pfd) ) // Try and set the pixel format based on our PFD  
		return false;

	
	HGLRC tempContext = wglCreateContext(hdc);
	wglMakeCurrent(hdc, tempContext);

	if( GLEW_OK != glewInit() )
		return false;

	glGenFramebuffers = glGenFramebuffersEXT;
	glBindFramebuffer = glBindFramebufferEXT;
	glGenRenderbuffers = glGenRenderbuffersEXT;
	glBindRenderbuffer = glBindRenderbufferEXT;
	glRenderbufferStorage = glRenderbufferStorageEXT;
	glFramebufferRenderbuffer = glFramebufferRenderbufferEXT;
	glCheckFramebufferStatus = glCheckFramebufferStatusEXT;
	glFramebufferTexture2D = glFramebufferTexture2DEXT;
#endif

	printf( "running opengl %s\n", glGetString( GL_VERSION ) );
    
    printf( "got toa  here....\n" );

	capabilities = OpenGLCapabilities( this );
	//capabilities.ForceDisablePixelBufferObject();
    
    glGetIntegerv( GL_FRAMEBUFFER_BINDING, &default_fbo_id );

		
	OpenGLRenderTarget::pGraphicsDevice = this;
	OpenGLTexture::pGraphicsDevice = this;
	SetViewport( width, height );
	pBoundVertexBuffer = nullptr;

	initialized = true;
	return true;
}

bool OpenGLGraphicsDevice::Draw( const unsigned int num_vertices )
{	
	if( pBoundVertexBuffer )
	{
		glBindBuffer(GL_ARRAY_BUFFER, pBoundVertexBuffer->buffer.vbo_id);
		const char* attributes[] = {"in_position", "in_color_uv", "in_normal", "in_tangent", "in_bitangent", nullptr };
		for( unsigned int i = 0; attributes[i]; i++ )
		{
			glEnableVertexAttribArray( i );
			glVertexAttribPointer( i, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*) nullptr + sizeof(float)*3*i);
		}

		glDrawArrays( pBoundVertexBuffer->GetOpenGLPrimitiveType(), 0, num_vertices );
		return true;
	}
	return false;
}

void OpenGLGraphicsDevice::SetViewport( const unsigned int width, const unsigned int height )
{
	BaseGraphicsDevice::SetViewport( width, height );
	glViewport( 0, 0, width, height );
}

OpenGLGraphicsDeviceStateManager& OpenGLGraphicsDevice::GetStateManager()
{
	return graphics_device_state_manager;	
}

OpenGLCapabilities& OpenGLGraphicsDevice::GetCapabilities()
{
	return capabilities;
}

void OpenGLGraphicsDevice::Clear( Color c )
{
	glClearColor( c.GetNormalizedRed(), c.GetNormalizedGreen(), c.GetNormalizedBlue(), c.GetNormalizedAlpha() );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );	
}

bool OpenGLGraphicsDevice::SwapBackBuffer()
{
#ifdef _WIN32	
	SwapBuffers(hdc);
#endif
	return true;
}

void OpenGLGraphicsDevice::SetVertexBuffer( OpenGLVertexBuffer& vertexBuffer )
{
	pBoundVertexBuffer = &vertexBuffer;
}

void OpenGLGraphicsDevice::SetVertexBuffer( OpenGLDynamicVertexBuffer& vertexBuffer )
{
	pBoundVertexBuffer = &vertexBuffer;
}

void OpenGLGraphicsDevice::SetDefaultRenderTarget()
{
	glBindFramebuffer( GL_FRAMEBUFFER, default_fbo_id );
}

void OpenGLGraphicsDevice::SetRenderTarget( OpenGLRenderTarget& render_target )
{
	glBindFramebuffer( GL_FRAMEBUFFER, render_target.fbo_id );
}

void OpenGLGraphicsDevice::SetRenderTarget( OpenGLRenderTarget& render_target, const Color& clear_color )
{
	SetRenderTarget( render_target );
	Clear( clear_color );
}

void OpenGLGraphicsDevice::RenderText( const unsigned int x, const unsigned int y, const unsigned int font_size, const std::string& font_name, const std::string& text )
{
	OpenGLTextRenderer( font_name, font_size ).Render( x, y, text );	
}
	
void OpenGLGraphicsDevice::RenderTextToBottomCenter( const unsigned int font_size, const std::string& font_name, const std::string& text )
{
	OpenGLTextRenderer( font_name, font_size ).RenderBottomCenter( viewport, text );
}

GeoFloat2 OpenGLGraphicsDevice::GetTextSize( const unsigned int font_size, const std::string& font_name, const std::string& text )
{
	return OpenGLTextRenderer( font_name, font_size ).GetSize( viewport, text );
}
