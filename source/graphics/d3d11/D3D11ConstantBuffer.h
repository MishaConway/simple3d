#pragma once

#include "D3D11Buffer.h"
#include "../BaseConstantBuffer.h"

template <class T>
class D3D11ConstantBuffer : public BaseConstantBuffer<T>
{
friend class D3D11Effect;
public:
	virtual bool Update()
	{
		if( !initialized )
		{
			char buf[1024];
			sprintf_s( buf, "sizeof t is %i\n", sizeof(T) );
			OutputDebugStringA( buf );

			buffer = D3D11Buffer( &data, sizeof(T), D3D11_BIND_CONSTANT_BUFFER, D3D11_USAGE_DYNAMIC );
			initialized = true;
		}

		buffer.SetData( &data );
		
		return true;
	}
	
private:
	D3D11Buffer buffer;
};
