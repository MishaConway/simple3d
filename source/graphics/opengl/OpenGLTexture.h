#pragma once

#include "include_opengl.h"
#include "soil/SOIL.h"
#include <string>
#include <map>
#include <vector>
#include "../BaseTexture.h"
#include "../../geo/GeoFloat.h"

namespace OpenGLTextureUsage
{
	enum OPENGL_TEXTURE_USAGE
	{
		shader_resource = 0,
		render_target
	};
}

class OpenGLGraphicsDevice;
class OpenGLTexture : public BaseTexture
{
friend class OpenGLGraphicsDevice;
public:
	OpenGLTexture();
	OpenGLTexture( const unsigned int width, const unsigned int height );
	OpenGLTexture( const unsigned int width, const unsigned int height, const Color& color );
	OpenGLTexture( const unsigned int width, const unsigned int height,  const OpenGLTextureUsage::OPENGL_TEXTURE_USAGE usage );
	OpenGLTexture( const std::string& image_filename );
	OpenGLTexture( OpenGLTexture a, OpenGLTexture b );
	OpenGLTexture( OpenGLTexture a, OpenGLTexture b, OpenGLTexture c );
	OpenGLTexture( std::vector<OpenGLTexture> textures );
	OpenGLTexture Clone( const unsigned width, const unsigned int height );
	GLint GetOpenGLTextureId();
	GLenum GetFormat();
	bool IsTextureArray();
	virtual bool IsFloatTexture();
	bool SaveToFile( const std::string& filename, const bool save_only_once = false );
protected:
	void* Map();
	void* Map( unsigned int* pPitch );
	void Unmap();
private:
	void PrepareTextureArray( std::vector<OpenGLTexture> textures );
	void Setup( const unsigned int width, const unsigned int height, const OpenGLTextureUsage::OPENGL_TEXTURE_USAGE usage );
	//unsigned int BitsPerPixel( DXGI_FORMAT fmt );
private:
	GLuint texture_id, pbo_id;
	GLenum format;
	GLenum internal_format, external_format;
	GLenum component_type;
	void* pMappedData;
	static OpenGLGraphicsDevice* pGraphicsDevice;
};
