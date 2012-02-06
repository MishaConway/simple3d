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
#ifdef _WIN32	
    const std::tr1::regex pattern("#include([^\n]+)");
	const std::tr1::sregex_token_iterator end;
	
	std::vector<std::string> include_directive_matches;
	for (std::tr1::sregex_token_iterator i(source.begin(), source.end(), pattern); i != end; ++i)
		include_directive_matches.push_back( (*i).str() );

	for( unsigned int i = 0; i < include_directive_matches.size(); i++ )
	{
		std::string include_file = StripString( include_directive_matches[i], "\"" );
		include_file = root_dir + ExplodeString( include_file, " \t" ).back();
		std::string include_source = File::ReadAllText( include_file );
		if( include_source.empty() )
			printf( "could not find include source for %s\n", include_file.c_str() );
		else
			source = StringReplace( source, include_directive_matches[i], File::ReadAllText( include_file ) );
	}
#endif

//	printf( "final source is %s\n", source.c_str() );
	return source;
}
 
OpenGLEffect::OpenGLEffect( const std::string& effect_path )
{
	std::vector<std::string> effect_path_parts = ExplodeString( effect_path, "/\\");
	effect_path_parts.pop_back();
	const std::string effect_directory = ImplodeVector( effect_path_parts, "/" ) + "/";	
	std::string source = File::ReadAllText( effect_path );
	
#ifdef _WIN32
	const std::tr1::regex pattern("technique(.)([^}]+)");

	const std::tr1::sregex_token_iterator end;
	for (std::tr1::sregex_token_iterator i(source.begin(), source.end(), pattern); i != end; ++i)
    {
		const std::string technique_source = (*i).str() + "}";
		 printf( "%s\n",  technique_source.c_str() );

		std::tr1::cmatch technique_name_results;
	    const std::tr1::regex technique_name_pattern("technique([^{]+)(([^)]+))");
		std::regex_search( technique_source.c_str(), technique_name_results, technique_name_pattern );

		std::string technique_name = technique_name_results[1].str().c_str();
		technique_name = std::regex_replace( technique_name, std::regex("[\\{\\}[:blank:]]"), std::string("") );

		std::string technique_body = technique_name_results[2].str().c_str();
		technique_body = std::regex_replace( technique_body, std::regex("[\\{\\}[:blank:]]"), std::string("") );

		std::vector<std::string> technique_body_parts = ExplodeString( technique_body, "," );

		const std::string vertex_shader_path = effect_directory + technique_body_parts[0] + ".vert";
		const std::string pixel_shader_path =  effect_directory + technique_body_parts[1] + ".frag";

		std::string vertex_shader_source = EvaluateIncludeDirectives(effect_directory, File::ReadAllText( vertex_shader_path ));
		std::string pixel_shader_source = EvaluateIncludeDirectives(effect_directory, File::ReadAllText( pixel_shader_path ));


			
		shader_programs[technique_name] = OpenGLShaderProgram( technique_name, OpenGLVertexShader( effect_directory + technique_body_parts[0] + ".vert", vertex_shader_source ), OpenGLFragmentShader( effect_directory + technique_body_parts[1] + ".frag", pixel_shader_source) ) ;
    }
#else
	//TODO: implement equivalent logic for systems that don't support regex....
#endif

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

