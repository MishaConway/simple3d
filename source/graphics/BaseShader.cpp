#include "BaseShader.h"


BaseShader::BaseShader()
{

}

BaseShader::BaseShader( const std::string& name, const std::string& path )
{
	valid = false;
	this->name = name;
}

std::string BaseShader::GetName()
{
	return name;
}

BaseShaderProgram::BaseShaderProgram()
{

}

std::string BaseShaderProgram::GetName()
{
	return name;
}