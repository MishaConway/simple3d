#pragma once

class IValidatable
{
public:
	IValidatable();
	virtual bool IsValid();
protected:
	bool valid;
};