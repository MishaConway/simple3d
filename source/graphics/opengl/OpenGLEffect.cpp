#include "OpenGLEffect.h"
#include "../../system/FileIO.h"
#include "../../string/string_utils.h"
#ifdef _WIN32
#include <regex>
#endif


OpenGLEffect OpenGLEffect::current_effect;

OpenGLEffect::OpenGLEffect()
{
	valid = false;
}

std::string OpenGLEffect::EvaluateIncludeDirectives( std::string root_dir, std::string source )
{        
    const std::vector<std::string> include_directive_matches = GetRegexMatches( source, "#include([^\n]+)" );    
    for( unsigned int i = 0; i < include_directive_matches.size(); i++ )
	{
		std::string include_file = StripString( include_directive_matches[i], "\"" );
		include_file = root_dir + ExplodeString( include_file, " \t" ).back();
		const std::string include_source = File::ReadAllText( include_file );
		if( include_source.empty() )
			printf( "could not find include source for %s\n", include_file.c_str() );
		else
			source = StringReplace( source, include_directive_matches[i], include_source );
	}
    
	return source;
}
 
OpenGLEffect::OpenGLEffect( const std::string& effect_path )
{
	std::vector<std::string> effect_path_parts = ExplodeString( effect_path, "/\\");
	effect_path_parts.pop_back();
	const std::string effect_directory = ImplodeVector( effect_path_parts, "/" ) + "/";	
	const std::string source = File::ReadAllText( effect_path );
    
    std::vector<std::string> regex_matches = GetRegexMatches( source, "technique([^}]+)" );
    for( unsigned int i = 0; i < regex_matches.size(); i++ )
    {
        const std::vector<std::string> split = ExplodeString( StringReplace( regex_matches[i], "technique", "" ), "{" );
        const std::string technique_name = Trim(split[0]);
        const std::vector<std::string> shaders = ExplodeString( split[1], "," );
        const std::string vertex_shader = Trim(shaders[0]);
        const std::string pixel_shader = Trim( shaders[1]);
        
        const std::string vertex_shader_path = effect_directory + vertex_shader + ".vert";
        const std::string pixel_shader_path = effect_directory + pixel_shader + ".frag";
        
        const std::string vertex_shader_source = EvaluateIncludeDirectives(effect_directory, File::ReadAllText(vertex_shader_path));
		const std::string pixel_shader_source = EvaluateIncludeDirectives(effect_directory, File::ReadAllText( pixel_shader_path ));
        
		shader_programs[technique_name] = OpenGLShaderProgram( technique_name, OpenGLVertexShader( vertex_shader, vertex_shader_source ), OpenGLFragmentShader( pixel_shader, pixel_shader_source) ) ;
    }
    
	std::vector<OpenGLShader> valid_shaders = OpenGLShader::GetValidShaders();
	for( unsigned int i = 0; i < valid_shaders.size(); i++ )
		printf( "valid shader: %s\n", valid_shaders[i].GetName().c_str() );

	std::vector<OpenGLShaderProgram> valid_shader_programs = OpenGLShaderProgram::GetValidShaderPrograms();
	for( unsigned int i = 0; i < valid_shader_programs.size(); i++ )
		printf( "valid shader program: %s\n", valid_shader_programs[i].GetName().c_str() );

	std::vector<OpenGLShaderProgram> invalid_shader_programs = OpenGLShaderProgram::GetInvalidShaderPrograms();
	for( unsigned int i = 0; i < invalid_shader_programs.size(); i++ )
		printf( "invalid shader program: %s\n", invalid_shader_programs[i].GetName().c_str() );

	const GLubyte* pShaderVersion = glGetString(GL_SHADING_LANGUAGE_VERSION); //undeclared identifier
	printf( "cool\n" );

	shader_programs["Basic"].Enable();

	Enable();
	printf( "enable program enable.\n" );

}

void OpenGLEffect::AddTechnique( OpenGLShaderProgram program )
{
    shader_programs[program.GetName()] = program;
}

 void OpenGLEffect::SetTechnique( const std::string& technique_name )
 {
     shader_programs[technique_name].Enable();
	 unsigned int mmm = matrix_uniforms.size();
	 for( std::map<std::string, GeoMatrix>::iterator it = matrix_uniforms.begin(); it != matrix_uniforms.end();  it++ )
		 shader_programs[technique_name].SetMatrix( it->first, it->second );
	 for( std::map<std::string, float>::iterator it = float_uniforms.begin(); it != float_uniforms.end();  it++ )
		 shader_programs[technique_name].SetFloat( it->first, it->second );
	 for( std::map<std::string, int>::iterator it = int_uniforms.begin(); it != int_uniforms.end();  it++ )
		 shader_programs[technique_name].SetInt( it->first, it->second );
	 for( std::map<std::string, std::vector<float> >::iterator it = float_array_uniforms.begin(); it != float_array_uniforms.end();  it++ )
		 shader_programs[technique_name].SetFloatArray( it->first, it->second );
	 unsigned int i = 0;
	 for( std::map<std::string, OpenGLTexture>::iterator it = texture_uniforms.begin(); it != texture_uniforms.end();  it++, i++ )
		 shader_programs[technique_name].SetTexture( it->first, it->second, i );
 }

 void OpenGLEffect::UnsetTechnique()
 {
	unsigned int i = 0;
	 for( std::map<std::string, OpenGLTexture>::iterator it = texture_uniforms.begin(); it != texture_uniforms.end();  it++, i++ )
	{
		glActiveTexture( GL_TEXTURE0 + i ); 
		glBindTexture( GL_TEXTURE_2D, 0 );
	}
	glActiveTexture( GL_TEXTURE0 ); 
 }
 
 #ifdef _WIN32	
 bool OpenGLEffect::RenderTechnique( const std::string& technique_name, portable_function<void()> f )
 {
	 SetTechnique( technique_name );
	 f(); 
	 UnsetTechnique();
	return true;
 }
#endif

 void OpenGLEffect::Enable()
 {
	current_effect = *this;		
 }

OpenGLEffect& OpenGLEffect::GetCurrentEffect()
{
	return current_effect;
}

 bool OpenGLEffect::SetInt( const std::string& variable_name, const int i )
 {
	 int_uniforms[variable_name] = i;
	 return true;
 }

 bool OpenGLEffect::SetFloat( const std::string& variable_name, float flt )
 {
	 float_uniforms[variable_name] = flt;
	 return true;
 }

#ifdef __XNAMATH_H__
 bool OpenGLEffect::SetFloatArray( const std::string& variable_name, XMFLOAT3& float_array )
 {
	 std::vector<float> f;
	 f.push_back( float_array.x );
	 f.push_back( float_array.y );
	 f.push_back( float_array.z );
	 float_array_uniforms[variable_name] = f;
	 return true;
	// return SetEffectVariable( variable_name, [&float_array](const GLint uniform_location){glUniform3fv( uniform_location, 1, (float*) &float_array );});
 }

 bool OpenGLEffect::SetFloatArray( const std::string& variable_name, XMFLOAT4& float_array )
 {
	 std::vector<float> f;
	 f.push_back( float_array.x );
	 f.push_back( float_array.y );
	 f.push_back( float_array.z );
	 f.push_back( float_array.w );
	 float_array_uniforms[variable_name] = f;
	 return true;
	 
	 //return SetFloatArray( variable_name, GeoVector( float_array ) );
 }
#endif 

 bool OpenGLEffect::SetFloatArray( const std::string& variable_name, GeoVector float_array )
 {
	 std::vector<float> f;
	 f.push_back( float_array.x );
	 f.push_back( float_array.y );
	 f.push_back( float_array.z );
	 f.push_back( float_array.w );
	 float_array_uniforms[variable_name] = f;
	 return true;
	 
	 // return SetEffectVariable( variable_name, [&float_array](const GLint uniform_location){glUniform4fv( uniform_location, 1, (float*) &float_array );});
 }

 bool OpenGLEffect::SetFloatArray( const std::string& variable_name, std::vector<float> float_array )
 {
	float_array_uniforms[variable_name] = float_array;
	return true;
 }

#ifdef __XNAMATH_H__
 bool OpenGLEffect::SetMatrix( const std::string& variable_name, XMMATRIX& matrix )
 {	 
	 return SetMatrix( variable_name, GeoMatrix( matrix ) );
 }

 bool OpenGLEffect::SetMatrix( const std::string& variable_name, XMFLOAT4X4& matrix )
 {
	 return SetMatrix( variable_name, GeoMatrix( matrix ) );
 }
#endif

 bool OpenGLEffect::SetMatrix( const std::string& variable_name, GeoMatrix matrix )
 {
	matrix_uniforms[variable_name] = matrix;
	return true;
	 
	 //return SetEffectVariable( variable_name, [&matrix](const GLint uniform_location){glUniformMatrix4fv( uniform_location, 1, GL_FALSE, (float*) &matrix );});
 }

 bool OpenGLEffect::SetTexture( const std::string& variable_name, OpenGLTexture texture )
 {
	SetFloat( variable_name + "_horizontal_offset", texture.GetHorizontalOffset() );
	SetFloat( variable_name + "_vertical_offset", texture.GetVerticalOffset() );
	SetFloat( variable_name + "_horizontal_scale", texture.GetHorizontalTiling() );
	SetFloat( variable_name + "_vertical_scale", texture.GetVerticalTiling() );
	SetFloat( variable_name + "_width", (float)texture.GetWidth() );
	SetFloat( variable_name + "_height", (float)texture.GetHeight() );
	SetInt( variable_name + "_enabled", texture.IsValid() ? 1 : 0 );
	texture_uniforms[variable_name] = texture;
	return true;
	/* return SetEffectVariable( variable_name, [&texture](const GLint uniform_location){
		 glUniform1i( uniform_location, 0 );
		glBindTexture( GL_TEXTURE_2D, texture.GetOpenGLTextureId() );
	 }); */
 }

