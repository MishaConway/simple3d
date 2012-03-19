#include "OpenGLShader.h"
#include "../../geo/Geometry.h"
#include <algorithm>
#include "../../std/nullptr.h"
#include "../../string/string_utils.h"

std::map<std::string, OpenGLShader> OpenGLShader::shaders;
OpenGLShaderProgram OpenGLShaderProgram::current_program;
std::map<std::string, OpenGLShaderProgram> OpenGLShaderProgram::shader_programs;

OpenGLShader::OpenGLShader()
{
	valid = false;
}

std::string OpenGLShader::GetName()
{
	return name;
}

std::vector<OpenGLShader> OpenGLShader::GetValidShaders()
{
	std::vector<OpenGLShader> valid_shaders;
	for( std::map<std::string, OpenGLShader>::iterator it = shaders.begin(); it != shaders.end();  it++ )
		if( it->second.IsValid() )
			valid_shaders.push_back( it->second );
	return valid_shaders;
}

OpenGLShader::OpenGLShader( const GLenum shader_type, const std::string& name, std::string source )
{
	valid = false;

	this->name = name;

	if( shaders.count( name ) )
	{
		shader_id = shaders[name].GetOpenGLShaderId();
		valid = shaders[name].IsValid();
	}
	else
	{
		if( !source.empty() )
		{				
			shader_id = glCreateShader( shader_type );

			#ifdef GL_ES_VERSION_2_0
			source = "precision highp float;\n" + source;
			#endif

			const char* c_str = source.c_str();
			const GLint length = source.length();
			glShaderSource( shader_id, 1, &c_str, &length );  
			glCompileShader(shader_id);	
	
			GLint compiled;
			glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled);
			if (compiled)
				valid = true;
			else
			{
				GLint blen = 0;	
				GLsizei slen = 0;
				glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH , &blen);       
				if (blen > 1)
				{
					GLchar* compiler_log = new GLchar[blen];
					#ifdef GL_ES_VERSION_2_0
					glGetShaderInfoLog(shader_id, blen, &slen, compiler_log);
					#else
					glGetInfoLogARB(shader_id, blen, &slen, compiler_log);
					#endif
					printf( "error compiling %s: %s\n", name.c_str(), compiler_log );
					delete [] compiler_log;
				}
			}

			shaders[name] = *this;
		}
	}
}

GLuint OpenGLShader::GetOpenGLShaderId()
{
	return shader_id;
}


OpenGLVertexShader::OpenGLVertexShader( std::string name, std::string source  ) : OpenGLShader( GL_VERTEX_SHADER, name, source )
{
    std::vector<std::string> regex_matches = GetRegexMatches( source, "attribute([^;]+)" );
    for( unsigned int i = 0; i < regex_matches.size(); i++ )
        input_attributes.push_back( ExplodeString( regex_matches[i], " \t\n" ).back() );
}

std::vector<std::string> OpenGLVertexShader::GetInputAttributes()
{
    return input_attributes;
}


OpenGLFragmentShader::OpenGLFragmentShader( std::string name, std::string source ) : OpenGLShader( GL_FRAGMENT_SHADER, name, source ){}

OpenGLShaderProgram::OpenGLShaderProgram()
{
	valid = false;
}

OpenGLShaderProgram::OpenGLShaderProgram( const std::string& name, OpenGLVertexShader vertex_shader, OpenGLFragmentShader fragment_shader )
{
	this->name = name;
	valid = false;

	if( vertex_shader.IsValid() && fragment_shader.IsValid() )
	{
        program_id = glCreateProgram();
		glAttachShader( program_id, vertex_shader.GetOpenGLShaderId() );
		glAttachShader( program_id, fragment_shader.GetOpenGLShaderId() );	

		input_attributes = vertex_shader.GetInputAttributes();
        for( unsigned int i = 0; i < input_attributes.size(); i++ )
        {
            printf( "binding attribute %s to slot %i\n", input_attributes[i].c_str(), i );
            glBindAttribLocation(program_id, i, input_attributes[i].c_str() );
        }
        
		glLinkProgram( program_id );
		GLint linked;
		glGetProgramiv(program_id, GL_LINK_STATUS, &linked);
		if (linked)
		{
			valid = true;
			printf( "successful program %s\n", name.c_str() );

			int total = -1;
			glGetProgramiv( program_id, GL_ACTIVE_UNIFORMS, &total ); 
			for(int i=0; i<total; ++i)  
			{
				int name_len=-1, num=-1;
				GLenum type = GL_ZERO;
				char name[256];
				glGetActiveUniform( program_id, GLuint(i), sizeof(name)-1, &name_len, &num, &type, name );
				name[name_len] = 0;
				printf( "adding used unifom %s\n", name );
				used_uniforms.push_back( StringReplace( std::string(name), "[0]", "") );
			}

			if( total > 0 )
			printf( "cool..\n" );


		}
		else
		{
			GLint blen = 0;	
			GLsizei slen = 0;
			glGetShaderiv(program_id, GL_INFO_LOG_LENGTH , &blen);       
			if (blen > 1)
			{
				GLchar* link_log = new GLchar[blen];
				#ifdef GL_ES_VERSION_2_0
				glGetProgramInfoLog(program_id, blen, &slen, link_log);
				#else
				glGetInfoLogARB(program_id, blen, &slen, link_log);
				#endif

				printf( "error linking program: %s\n", link_log );
				delete [] link_log;
			}
		} 
	}

	shader_programs[name] = *this;
}

void OpenGLShaderProgram::Enable()
{
	glUseProgram( program_id );
    for( unsigned int i = 0; i < input_attributes.size(); i++ )
    {
        glEnableVertexAttribArray( i );
        glVertexAttribPointer( i, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (char*) nullptr + sizeof(float)*3*i);
    }
	current_program = *this;
}

GLuint OpenGLShaderProgram::GetProgramId()
{
	return program_id;
}

std::string OpenGLShaderProgram::GetName()
{
	return name;
}

std::vector<std::string> OpenGLShaderProgram::GetUsedUniforms()
{
	return used_uniforms;
}

std::vector<std::string> OpenGLShaderProgram::GetInputAttributes()
{
    return input_attributes;
}

bool OpenGLShaderProgram::UsingUniform( const std::string& uniform_name )
{
	return used_uniforms.end() != std::find( used_uniforms.begin(), used_uniforms.end(), uniform_name );
}

OpenGLShaderProgram OpenGLShaderProgram::GetCurrentProgram()
{
	return current_program;
}

std::vector<OpenGLShaderProgram> OpenGLShaderProgram::GetValidShaderPrograms()
{
	std::vector<OpenGLShaderProgram> valid_shaders_programs;
	for( std::map<std::string, OpenGLShaderProgram>::iterator it = shader_programs.begin(); it != shader_programs.end();  it++ )
		if( it->second.IsValid() )
			valid_shaders_programs.push_back( it->second );
	return valid_shaders_programs;
}

std::vector<OpenGLShaderProgram> OpenGLShaderProgram::GetInvalidShaderPrograms()
{
	std::vector<OpenGLShaderProgram> invalid_shaders_programs;
	for( std::map<std::string, OpenGLShaderProgram>::iterator it = shader_programs.begin(); it != shader_programs.end();  it++ )
		if( !it->second.IsValid() )
			invalid_shaders_programs.push_back( it->second );
	return invalid_shaders_programs;
}

GLint OpenGLShaderProgram::GetUniformLocation( const std::string& variable_name )
{
	if( UsingUniform( variable_name ) )
	{
		const GLint uniform_location = glGetUniformLocation( GetProgramId(), variable_name.c_str() );
		if( -1 == uniform_location )
		{
			printf( "error with %s program getting uniform location for %s\n", GetName().c_str(), variable_name.c_str() );
			printf( "oh no\n" );	
		}
		return uniform_location;
	}

	return -1;
}

#ifdef _WIN32	
bool OpenGLShaderProgram::SetEffectVariable( const std::string& variable_name, portable_function<void(const GLint uniform_location)> f )
#endif
#if defined(__APPLE__) || defined(__APPLE_CC__)  
bool OpenGLShaderProgram::SetEffectVariable( const std::string& variable_name, void(^f)(const GLint uniform_location) )
#endif
{
	GLint uniform_location = GetUniformLocation( variable_name );
	if( -1 != uniform_location )
		f( uniform_location );
	return true;
}

 bool OpenGLShaderProgram::SetInt( const std::string& variable_name, const int i )
 {
#ifdef _WIN32	
	 return SetEffectVariable( variable_name, [&i](const GLint uniform_location){
#endif
#if defined(__APPLE__) || defined(__APPLE_CC__)  
      return SetEffectVariable( variable_name, ^(const GLint uniform_location){                      
#endif
     glUniform1i( uniform_location, i );});
 }

 bool OpenGLShaderProgram::SetFloat( const std::string& variable_name, float flt )
 {
#ifdef _WIN32		 
	 return SetEffectVariable( variable_name, [&flt](const GLint uniform_location){
#endif
#if defined(__APPLE__) || defined(__APPLE_CC__)  
     return SetEffectVariable( variable_name, ^(const GLint uniform_location){                      
#endif
    glUniform1f( uniform_location, flt );});
 }

#ifdef __XNAMATH_H__
 bool OpenGLShaderProgram::SetFloatArray( const std::string& variable_name, XMFLOAT3& float_array )
 {
	  return SetEffectVariable( variable_name, [&float_array](const GLint uniform_location){glUniform3fv( uniform_location, 1, (float*) &float_array );});
 }

 bool OpenGLShaderProgram::SetFloatArray( const std::string& variable_name, XMFLOAT4& float_array )
 {
	 return SetFloatArray( variable_name, GeoVector( float_array ) );
 }
#endif

 bool OpenGLShaderProgram::SetFloatArray( const std::string& variable_name, GeoVector& float_array )
 {
#ifdef _WIN32		 
	 return SetEffectVariable( variable_name, [&float_array](const GLint uniform_location){
#endif
#if defined(__APPLE__) || defined(__APPLE_CC__)  
    return SetEffectVariable( variable_name, ^(const GLint uniform_location){                      
#endif
    glUniform4fv( uniform_location, 1, (float*) &float_array );});
 }

 bool OpenGLShaderProgram::SetFloatArray( const std::string& variable_name, std::vector<float>& float_array )
 {
#ifdef _WIN32		 
	 return SetEffectVariable( variable_name, [&float_array, &variable_name](const GLint uniform_location){
#endif
#if defined(__APPLE__) || defined(__APPLE_CC__)  
     return SetEffectVariable( variable_name, ^(const GLint uniform_location){                      
#endif
    if( variable_name == "color" )
        glUniform4fv( uniform_location, 1, &float_array[0] );
    else
         glUniform1fv( uniform_location, float_array.size(),  &float_array[0]);});
 }

#ifdef __XNAMATH_H__
 bool OpenGLShaderProgram::SetMatrix( const std::string& variable_name, XMMATRIX& matrix )
 {
	 return SetMatrix( variable_name, GeoMatrix( matrix ) );
 }

 bool OpenGLShaderProgram::SetMatrix( const std::string& variable_name, XMFLOAT4X4& matrix )
 {
	 return SetMatrix( variable_name, GeoMatrix( matrix ) );
 }
#endif

 bool OpenGLShaderProgram::SetMatrix( const std::string& variable_name, GeoMatrix& matrix )
 {
#ifdef _WIN32		 
	 return SetEffectVariable( variable_name, [&matrix](const GLint uniform_location){
#endif
#if defined(__APPLE__) || defined(__APPLE_CC__)  
     return SetEffectVariable( variable_name, ^(const GLint uniform_location){                      
#endif
     glUniformMatrix4fv( uniform_location, 1, GL_FALSE, (float*) &matrix );});
 }

 bool OpenGLShaderProgram::SetTexture( const std::string& variable_name, OpenGLTexture& texture, const GLenum texture_index )
 {
	  glActiveTexture( GL_TEXTURE0 + texture_index );
	  glEnable( GL_TEXTURE_2D );
	  glBindTexture( GL_TEXTURE_2D, texture.GetOpenGLTextureId() );
	  glActiveTexture( GL_TEXTURE0 ); 
#ifdef _WIN32
	 return SetEffectVariable( variable_name, [&texture, &texture_index](const GLint uniform_location){
#endif
#if defined(__APPLE__) || defined(__APPLE_CC__)  
     return SetEffectVariable( variable_name, ^(const GLint uniform_location){                      
#endif
      glUniform1i( uniform_location, texture_index );});
 }
