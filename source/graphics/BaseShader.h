#include <string>
#include <map>
#include "../interfaces/IValidatable.h"

class BaseShader : public IValidatable
{
public:
	BaseShader();
	static BaseShader FromFile( const std::string filename );

	std::string GetName();
protected:
	std::string name;
	static std::map<std::string, BaseShader*> shaders;
};