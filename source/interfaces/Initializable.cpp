#include "IInitializable.h"

IInitializable::IInitializable()
{
	initialized = false;
}

bool IInitializable::IsInitialized()
{
	return initialized;	
}