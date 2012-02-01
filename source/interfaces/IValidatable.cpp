#include "IValidatable.h"

IValidatable::IValidatable()
{
	valid = false;
}
	
bool IValidatable::IsValid()
{
	return valid;
}