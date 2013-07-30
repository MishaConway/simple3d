#ifdef _WIN32
#include "D3D11Buffer.h"
#include "D3D11GraphicsDevice.h"

D3D11GraphicsDevice* D3D11Buffer::pGraphicsDevice = nullptr;

D3D11Buffer::D3D11Buffer()
{
	pDevice = nullptr;
	pBuffer = nullptr;
	valid = false;
}

D3D11Buffer::D3D11Buffer( void* pData, const unsigned int data_size, const unsigned int bind_flags, D3D11_USAGE usage = D3D11_USAGE_DEFAULT )
{
		char buffer[1024];
	sprintf_s( buffer, "width and height are %f, %f\n", D3D11Buffer::pGraphicsDevice->GetViewport().Width, D3D11Buffer::pGraphicsDevice->GetViewport().Height );
	OutputDebugStringA( buffer );
	
	this->pDevice = pGraphicsDevice->GetInternals().pDevice;
	this->pDeviceContext = pGraphicsDevice->GetInternals().pDeviceContext;


	



	
	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof(bd) );
	bd.Usage = usage;
	bd.ByteWidth = data_size;
	bd.BindFlags = bind_flags;
	if( D3D11_USAGE_DEFAULT == usage )
		bd.CPUAccessFlags = 0;
	else
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	//if( D3D11_BIND_CONSTANT_BUFFER == bind_flags )
	//	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory( &InitData, sizeof(InitData) );
	InitData.pSysMem = pData;

	if( pGraphicsDevice && pGraphicsDevice->IsInitialized() )
		printf( "it is initialized...\n" );
	else
		printf( "it is not initialized..\n" );


	if( FAILED( pDevice->CreateBuffer( &bd, pData ? &InitData : nullptr, &pBuffer ) ) )
	{
		printf( "unable to create buffer!\n" );
	}
	else
		valid = true;
}

void* D3D11Buffer::Map()
{
	if(FAILED(pDeviceContext->Map( pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource )))
	{
		printf("failed to map textre\n");
		return 0;
	}
	return mapped_resource.pData;
}

void D3D11Buffer::Unmap()
{
	pDeviceContext->Unmap( pBuffer, 0 );
}

bool D3D11Buffer::SetData( void* pData  )
{
	D3D11_BUFFER_DESC buffer_desc;
	pBuffer->GetDesc( &buffer_desc );
	void* pMappedData = Map();
	memcpy( mapped_resource.pData, pData, buffer_desc.ByteWidth ); 
	Unmap();
	return true;
}

#endif
