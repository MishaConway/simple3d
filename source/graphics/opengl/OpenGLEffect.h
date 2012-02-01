#pragma once

#include "../../geo/GeoVector.h"
#include "../../geo/GeoMatrix.h"
#include <string>
#include <vector>
#include "OpenGLTexture.h"
#include "OpenGLShader.h"
#include "../BaseEffect.h"
#include <map>

class OpenGLEffect : public BaseEffect
{
public:
 OpenGLEffect();
 OpenGLEffect( const std::string& effect_path );
 bool RenderTechnique( const std::string& technique_name, std::function<void()> f );

 void Enable();
 static OpenGLEffect& GetCurrentEffect();

 bool SetInt( const std::string& variable_name, const int i );
 bool SetFloat( const std::string& variable_name, float flt );
 bool SetFloatArray( const std::string& variable_name, XMFLOAT3& float_array );
 bool SetFloatArray( const std::string& variable_name, XMFLOAT4& float_array );
 bool SetFloatArray( const std::string& variable_name, GeoVector& float_array );
 bool SetFloatArray( const std::string& variable_name, std::vector<float>& float_array );
 bool SetMatrix( const std::string& variable_name, XMMATRIX& matrix );
 bool SetMatrix( const std::string& variable_name, XMFLOAT4X4& matrix );
 bool SetMatrix( const std::string& variable_name, GeoMatrix& matrix );
 bool SetTexture( const std::string& variable_name, OpenGLTexture& texture );	 
private:
	std::map< std::string, OpenGLShaderProgram > shader_programs;
	std::map< std::string, GeoMatrix > matrix_uniforms;
	std::map< std::string, int > int_uniforms;
	std::map< std::string, float > float_uniforms;
	std::map< std::string, OpenGLTexture> texture_uniforms;
	std::map< std::string, std::vector<float>> float_array_uniforms;

  static OpenGLEffect current_effect;
};