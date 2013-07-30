#ifdef _WIN32
#include "D3D11Effect.h"
#include "D3D11GraphicsDevice.h"

D3D11Effect D3D11Effect::current_effect;
D3D11GraphicsDevice* D3D11Effect::pGraphicsDevice = nullptr;

void D3D11Effect::Enable()
{
	current_effect = *this;
}

D3D11Effect D3D11Effect::GetCurrentEffect()
{
	return current_effect;
}

D3D11Effect::D3D11Effect(){valid=false;}
D3D11Effect::D3D11Effect( const std::string& effect_path ) : BaseEffect( effect_path )
{
	
	
	
	valid = false;

	std::vector< std::pair< std::string, std::string > > techniques = ParseTechniqueFile( effect_path );

	printf( "went here...\n" );


	
	/*
	this->pDeviceContext = pGraphicsDevice->GetInternals().pDeviceContext;

	ID3D10Blob* pShaderBlob = nullptr;
	ID3D10Blob* pErrorsBlob = nullptr;
	if(FAILED(D3DX11CompileEffectFromFile( L"", NULL , NULL , D3DCOMPILE_DEBUG , 0,  pGraphicsDevice->GetInternals().pDevice , &pEffect , &pErrorsBlob)))
	{
		printf( "could not compile shader file %s\n", effect_path.c_str() );
		if(pErrorsBlob)
		{
			printf( "%s", pErrorsBlob->GetBufferPointer() );
		}
		return;
	}



	
	// Traverse all Techniques
	unsigned int tIndex = 0;
	ID3DX11EffectTechnique* tech = NULL;
	while((tech = pEffect->GetTechniqueByIndex(tIndex++))->IsValid())
	{	
		// Traverse all passes	
		unsigned int pIndex = 0;	
		ID3DX11EffectPass* pass = NULL;	

		while((pass = tech->GetPassByIndex(pIndex++))->IsValid())	
		{							
			D3DX11_PASS_SHADER_DESC vs_desc;
			pass->GetVertexShaderDesc(&vs_desc);		
			if(0 == vs_desc.pShaderVariable)			
				throw "Shouldn't be zero";		
			D3DX11_EFFECT_SHADER_DESC effect_shader_description;
			vs_desc.pShaderVariable->GetShaderDesc(0, &effect_shader_description);
			if( FAILED( pGraphicsDevice->GetInternals().pDevice->CreateInputLayout( d3d11_vertex_layout, VERTEX_LAYOUT_NUM_ELEMENTS, effect_shader_description.pBytecode, effect_shader_description.BytecodeLength, &pVertexLayout ) ) )
			{
				printf("could not create vertex layout!\n");
				return;
			}
			else
				printf("input layout was created!!!");
		}
	}

	// Set the input layout
	pDeviceContext->IASetInputLayout( pVertexLayout );

	valid = true;

	if( !GetCurrentEffect().IsValid() )
		Enable();
		*/

	printf( "shader effect built!" );
}

bool D3D11Effect::SetEffectVariable( const std::string& variable_name, std::function<void(ID3DX11EffectVariable*)> f )
{
	if( !valid )
		return false;
	ID3DX11EffectVariable* pEffectVariable = pEffect->GetVariableByName( variable_name.c_str() );
	if( !pEffectVariable->IsValid() )
	{
		printf( "invalid effect variable %s\n", variable_name.c_str() );
		return false;
	}
	f(pEffectVariable);
	return true;
}

bool D3D11Effect::SetFloat( const std::string& variable_name, float flt )
{
	return false;
	//return SetEffectVariable( variable_name, [&flt](ID3DX11EffectVariable* pEffectVariable){ 
	// pEffectVariable->AsScalar()->SetFloat( flt );});
}

 bool D3D11Effect::SetInt( const std::string& variable_name, const int i )
 {
	return false;
	 //return SetEffectVariable( variable_name, [&i](ID3DX11EffectVariable* pEffectVariable){ 
	// pEffectVariable->AsScalar()->SetInt( i );});	
 }

bool D3D11Effect::SetFloatArray( const std::string& variable_name, XMFLOAT3& float_array )
{
	return false;
	//XMFLOAT4 f( float_array.x, float_array.y, float_array.z, 0 );
	//return SetFloatArray( variable_name, f );
	//return SetEffectVariable( variable_name, [&float_array](ID3DX11EffectVariable* pEffectVariable){ 
	// pEffectVariable->AsVector()->SetFloatVector( (float*) &float_array );});	
}

bool D3D11Effect::SetFloatArray( const std::string& variable_name, std::vector<float>& float_array )
{
	return false;
	//return SetEffectVariable( variable_name, [&float_array](ID3DX11EffectVariable* pEffectVariable){ 
	// pEffectVariable->AsScalar()->SetFloatArray( (float*) &float_array[0], 0, float_array.size() );});		
}

bool D3D11Effect::SetFloatArray( const std::string& variable_name, XMFLOAT4& float_array )
{
	return false;
	//return SetEffectVariable( variable_name, [&float_array](ID3DX11EffectVariable* pEffectVariable){ 
	// pEffectVariable->AsVector()->SetFloatVector( (float*) &float_array );});	
}

bool D3D11Effect::SetFloatArray( const std::string& variable_name, GeoVector float_array )
{
	return false;
	//return SetEffectVariable( variable_name, [&float_array](ID3DX11EffectVariable* pEffectVariable){ 
	// pEffectVariable->AsVector()->SetFloatVector( (float*) &float_array );});	
}

bool D3D11Effect::SetMatrix( const std::string& variable_name, XMMATRIX& matrix )
{
	return false;
	//return SetEffectVariable( variable_name, [&matrix](ID3DX11EffectVariable* pEffectVariable){ 
	// pEffectVariable->AsMatrix()->SetMatrix( (float*) &matrix ); });
}

bool D3D11Effect::SetMatrix( const std::string& variable_name, XMFLOAT4X4& matrix )
{
	return false;
	//return SetEffectVariable( variable_name, [&matrix](ID3DX11EffectVariable* pEffectVariable){ 
	// pEffectVariable->AsMatrix()->SetMatrix( (float*) &matrix ); });	
}

 bool D3D11Effect::SetMatrix( const std::string& variable_name, GeoMatrix& matrix )
 {
	return false;
	// return SetEffectVariable( variable_name, [&matrix](ID3DX11EffectVariable* pEffectVariable){ 
	// pEffectVariable->AsMatrix()->SetMatrix( (float*) &matrix ); });	
 }

bool D3D11Effect::SetTexture( const std::string& variable_name, D3D11Texture& texture )
{
	SetFloat( variable_name + "_horizontal_offset", texture.GetHorizontalOffset() );
	SetFloat( variable_name + "_vertical_offset", texture.GetVerticalOffset() );
	SetFloat( variable_name + "_horizontal_scale", texture.GetHorizontalTiling() );
	SetFloat( variable_name + "_vertical_scale", texture.GetVerticalTiling() );
	SetFloat( variable_name + "_width", (float)texture.GetWidth() );
	SetFloat( variable_name + "_height", (float)texture.GetHeight() );
	SetInt( "use_lala_array", texture.IsValid() && texture.IsTextureArray() ? 1 : 0 );
	return false;
	//return SetEffectVariable( variable_name, [&texture](ID3DX11EffectVariable* pEffectVariable){ 
		//pEffectVariable->AsShaderResource()->SetResource( texture.IsValid() ? texture.GetShaderResource().GetResourceView() : nullptr ); });	
}

bool D3D11Effect::RenderTechnique( const std::string& technique_name, std::function<void()> f )
{
	if( !valid )
		return false;
	
	return false;
	/*ID3DX11EffectTechnique* pTechnique;
	pTechnique = pEffect->GetTechniqueByName( technique_name.c_str() );
	D3DX11_TECHNIQUE_DESC technique_description;
	if( FAILED(pTechnique->GetDesc( &technique_description ) ) )
		printf( "could not get technique description\n" );
	bool success = std::string( technique_description.Name ) == technique_name;

	if( success )
	{	 
		for( UINT p = 0; p < technique_description.Passes; ++p )
		{
			if( FAILED(pTechnique->GetPassByIndex( p )->Apply(0, pDeviceContext)))
				printf("applying pass failed!\n" );
			f();
		}
	}
	else
	{
		printf("unable to set technique" );
	} 
	return success;*/
}
#endif
