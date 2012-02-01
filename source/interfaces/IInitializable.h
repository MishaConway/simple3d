#pragma once

class IInitializable
{
public:
	IInitializable();
	virtual bool IsInitialized();
protected:
	bool initialized;
};