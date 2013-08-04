#pragma once

#include <string>
#include <map>
#include "../interfaces/IValidatable.h"

class BaseShader : public IValidatable
{
public:
	BaseShader();
	BaseShader( const std::string& name, const std::string& path );

	std::string GetName();
protected:
	std::string name;
};


class BaseShaderProgram : public IValidatable
{
public:
	BaseShaderProgram();
	std::string GetName();
	virtual void Enable() = 0;
protected:
	std::string name;
};