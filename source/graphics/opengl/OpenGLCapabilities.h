#pragma once
#include "include_opengl.h"

class OpenGLGraphicsDevice;
class OpenGLCapabilities
{
public:	
	OpenGLCapabilities();
	OpenGLCapabilities( OpenGLGraphicsDevice* open_gl_graphics_device );
	bool SupportsPixelBufferObject();
	bool SupportsVertexBufferObject();
	bool SupportsNonPowerOfTwo();
	bool SupportsFloatTexture();
	bool SupportsTextureArray();
	void ForceDisablePixelBufferObject();
private:
	bool supports_pixel_buffer_object;
	bool supports_vertex_buffer_object;
	bool supports_non_power_of_two;
	bool supports_float_texture;
	bool supports_texture_array;
};