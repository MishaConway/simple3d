#pragma once

#include "../geo/GeoVector.h"
#include "../geo/GeoMatrix.h"
#include "Color.h"
#include <string>
#include <vector>
#include <map>
#include <functional>
#include "../interfaces/IValidatable.h"
#include "BaseShader.h"
#ifdef _WIN32
#include <regex>
#endif

#ifdef _WIN32
#define BEGIN_RENDER_TECHNIQUE(a, technique) a.RenderTechnique(technique,[this](){
#define BEGIN_RENDER_CURRENT_TECHNIQUE(a) a.RenderTechnique(GetTechnique(),[this](){ 
#endif

#if defined(__APPLE__) || defined(__APPLE_CC__) 
#define BEGIN_RENDER_TECHNIQUE(a, technique) a.RenderTechnique(technique,^(){
#define BEGIN_RENDER_CURRENT_TECHNIQUE(a) a.RenderTechnique(GetTechnique(),^(){  
#endif

#define END_RENDER_TECHNIQUE });

class BaseEffect : public IValidatable
{
public:
	BaseEffect();
	BaseEffect( const std::string& effect_path );
	virtual bool SetFloatArray( const std::string& variable_name, GeoVector float_array ) = 0;
	virtual bool SetColor( const std::string& variable_name, Color color );
protected:
     std::vector< std::pair< std::string, std::pair< std::string, std::string >> > ParseTechniqueFile( const std::string& technique_file );
protected:
	std::map< std::string, std::reference_wrapper<BaseShaderProgram> > programs;
};