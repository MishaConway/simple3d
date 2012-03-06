#pragma once
#include <string>

class IStringable
{
public:
	virtual std::string ToString() = 0;
};
