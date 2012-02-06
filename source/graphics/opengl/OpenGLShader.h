#pragma once

#include "include_opengl.h"
#include "../../system/FileIO.h"
#include <map>
#include "../../geo/Geometry.h"
#include "OpenGLTexture.h"
#include "../../interfaces/IValidatable.h"

class OpenGLShader : public IValidatable
{
public:
	OpenGLShader();
	GLuint GetOpenGLShaderId();
	static std::vector<OpenGLShader> GetValidShaders();
	std::string GetName();
protected:
	OpenGLShader( GLenum shader_type, const std::string& name, std::string source );
protected:
	GLuint shader_id;
	std::string name;
	static std::map<std::string, OpenGLShader> shaders;
};

class OpenGLVertexShader : public OpenGLShader
{
public:
    OpenGLVertexShader( std::string name, std::string source );
};

class OpenGLFragmentShader : public OpenGLShader
{
public:
    OpenGLFragmentShader( std::string name, std::string source );
};

class OpenGLShaderProgram : public IValidatable
{
public:
	OpenGLShaderProgram();
	OpenGLShaderProgram( const std::string& name, OpenGLVertexShader vertex_shader, OpenGLFragmentShader fragment_shader );
	static OpenGLShaderProgram GetCurrentProgram();
	static std::vector<OpenGLShaderProgram> GetValidShaderPrograms();
	static std::vector<OpenGLShaderProgram> GetInvalidShaderPrograms();
	GLuint GetProgramId();
	std::string GetName();
	std::vector<std::string> GetUsedUniforms();
	bool UsingUniform( const std::string& uniform_name );
	void Enable();
public:
	 bool SetInt( const std::string& variable_name, const int i );
	 bool SetFloat( const std::string& variable_name, float flt );
#ifdef __XNAMATH_H__
	 bool SetFloatArray( const std::string& variable_name, XMFLOAT3& float_array );
	 bool SetFloatArray( const std::string& variable_name, XMFLOAT4& float_array );
#endif
	 bool SetFloatArray( const std::string& variable_name, GeoVector& float_array );
	 bool SetFloatArray( const std::string& variable_name, std::vector<float>& float_array );
#ifdef __XNAMATH_H__
	 bool SetMatrix( const std::string& variable_name, XMMATRIX& matrix );
	 bool SetMatrix( const std::string& variable_name, XMFLOAT4X4& matrix );
#endif
	 bool SetMatrix( const std::string& variable_name, GeoMatrix& matrix );
	 bool SetTexture( const std::string& variable_name, OpenGLTexture& texture, const GLenum texture_index );
protected:
#ifdef _WIN32	
	bool SetEffectVariable( const std::string& variable_name, std::function<void(const GLint uniform_location)> f );
#endif
#if defined(__APPLE__) || defined(__APPLE_CC__)  
    bool SetEffectVariable( const std::string& variable_name, void(^f)(const GLint uniform_location) );
#endif    
	GLint GetUniformLocation( const std::string& variable_name );
protected:
	static OpenGLShaderProgram current_program;
	GLuint program_id;
	std::string name;
	std::vector<std::string> used_uniforms;
	static std::map<std::string, OpenGLShaderProgram> shader_programs;
};