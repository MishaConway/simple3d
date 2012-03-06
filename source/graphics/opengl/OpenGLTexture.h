#pragma once

#include "include_opengl.h"
#ifndef GL_ES_VERSION_2_0	
#include "soil/SOIL.h"
#endif
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
    
#if defined(__APPLE__) || defined(__APPLE_CC__)  
    static void SetOnLoadTextureFileBlock( unsigned char* (^load_texture_file)(const char* path, unsigned int* pOutWidth, unsigned int* pOutHeight) );
    static void SetOnSaveTextureFileBlock( void (^save_texture_file)(const char* path, unsigned char* data, const unsigned int width, const unsigned int height) );
    
    static void SetCreateTextureFromTextBlock( unsigned char* (^create_texture_from_text)(const char* font, const unsigned int font_size, const char* text, unsigned int* pOutWidth, unsigned int* pOutHeight) );
#endif
    
    
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
	virtual unsigned char* Map( unsigned int* pPitch );
	virtual void Unmap();
private:
	void PrepareTextureArray( std::vector<OpenGLTexture> textures );
	void Setup( const unsigned int width, const unsigned int height, const OpenGLTextureUsage::OPENGL_TEXTURE_USAGE usage );
	//unsigned int BitsPerPixel( DXGI_FORMAT fmt );
private:
	GLuint texture_id, pbo_id;
	GLenum format;
	GLenum internal_format, external_format;
	GLenum component_type;
	unsigned char* pMappedData;
	static OpenGLGraphicsDevice* pGraphicsDevice;
#if defined(__APPLE__) || defined(__APPLE_CC__)  
    static unsigned char* (^load_texture_file_block)(const char* path, unsigned int* pOutWidth, unsigned int* pOutHeight);
    static unsigned char* (^create_texture_from_text_block)(const char* font, const unsigned int font_size, const char* text, unsigned int* pOutWidth, unsigned int* pOutHeight);
    static void (^save_texture_file_block)(const char* path, unsigned char* data, const unsigned int width, const unsigned int height);
#endif
};
