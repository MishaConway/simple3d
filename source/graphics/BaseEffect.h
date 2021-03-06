#pragma once

#include "../geo/GeoVector.h"
#include "../geo/GeoMatrix.h"
#include "Color.h"
#include <string>
#include <vector>
#include <functional>
#include "../interfaces/IValidatable.h"

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
	virtual bool SetFloatArray( const std::string& variable_name, GeoVector float_array ) = 0;
	virtual bool SetColor( const std::string& variable_name, Color color );
};