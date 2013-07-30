#include "D3D11Buffer.h"

template <class T>
class BaseConstantBuffer
{
public:
	BaseConstantBuffer()
	{
		initialized = false;
	}

	virtual bool Update() = 0;
	T data;
protected:
	bool initialized;
};