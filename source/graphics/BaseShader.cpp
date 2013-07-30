#include "BaseShader.h"

BaseShader::BaseShader()
{

}

BaseShader BaseShader::FromFile( const std::string filename )
{
	BaseShader c;	
	return c;
}

std::string BaseShader::GetName()
{
	return name;
}