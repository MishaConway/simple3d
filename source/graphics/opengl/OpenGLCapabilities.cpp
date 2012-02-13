#include "OpenGLCapabilities.h"
#include <stdio.h>
#include "../../string/string_utils.h"

OpenGLCapabilities::OpenGLCapabilities(){}
OpenGLCapabilities::OpenGLCapabilities( OpenGLGraphicsDevice* open_gl_graphics_device )
{	
#ifdef _WIN32	
	supports_vertex_buffer_object = 1 == GLEW_ARB_vertex_buffer_object;
	supports_float_texture = 1 == GLEW_ARB_texture_non_power_of_two;
	supports_pixel_buffer_object = 1 == GLEW_ARB_pixel_buffer_object;
	supports_non_power_of_two = 1 == GLEW_ARB_pixel_buffer_object;
	supports_texture_array = 1 == GLEW_EXT_texture_array;
#endif

#ifdef GL_ES_VERSION_2_0
	supports_vertex_buffer_object = true; //vertex buffers are defined in core opengl ES spec
	supports_pixel_buffer_object = false; //pixel buffer objects are not supported in opengl ES spec
    supports_texture_array = false;
	std::vector<std::string> extensions = ExplodeString( std::string( (char*) glGetString (GL_EXTENSIONS) ), " " );
	//TODO: manually verify extensions here....
#endif
}

bool OpenGLCapabilities::SupportsPixelBufferObject()
{
	return supports_pixel_buffer_object;
}

bool OpenGLCapabilities::SupportsVertexBufferObject()
{
	return supports_vertex_buffer_object;
}
	
bool OpenGLCapabilities::SupportsNonPowerOfTwo()
{
	return supports_non_power_of_two;
}
	
bool OpenGLCapabilities::SupportsFloatTexture()
{
	return supports_float_texture;
}

bool OpenGLCapabilities::SupportsTextureArray()
{
	return supports_texture_array;
}

void OpenGLCapabilities::ForceDisablePixelBufferObject()
{
	supports_pixel_buffer_object = false;
}

