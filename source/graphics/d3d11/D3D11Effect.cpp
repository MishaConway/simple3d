#ifdef _WIN32
#include "D3D11Effect.h"
#include "D3D11GraphicsDevice.h"
#include "../../system/FileIO.h"

D3D11Effect D3D11Effect::current_effect;
D3D11GraphicsDevice* D3D11Effect::pGraphicsDevice = nullptr;


void D3D11Effect::Enable()
{
	current_effect = *this;
}

D3D11Effect& D3D11Effect::GetCurrentEffect()
{
	return current_effect;
}

D3D11Effect::D3D11Effect(){valid=false;}
D3D11Effect::D3D11Effect( const std::string& effect_path ) : BaseEffect( effect_path )
{	
	valid = false;

	auto techniques = ParseTechniqueFile( effect_path );
	for( unsigned int i = 0; i < techniques.size(); i++ )
	{
		const std::string name = techniques[i].first;
		const std::string vertex_shader_path = techniques[i].second.first;
		const std::string pixel_shader_path = techniques[i].second.second;

		D3D11VertexShader vertex_shader = D3D11VertexShader( vertex_shader_path, vertex_shader_path );
		D3D11PixelShader pixel_shader = D3D11PixelShader( pixel_shader_path, pixel_shader_path );

		//programs[name] = D3D11ShaderProgram( name, vertex_shader, pixel_shader );
		programs[name] = D3D11ShaderProgram( name, vertex_shader, pixel_shader );
	}

		for( auto it = programs.begin(); it != programs.end();  it++ )
			if( it->second.IsValid() )
				debug_print( "valid program: %s\n", it->second.GetName().c_str() );
			else
				debug_print( "invalid program: %s\n", it->second.GetName().c_str() );

		debug_print( "made it here... lala: %s\n", "haha" );
		debug_print("cc\n" );
	valid = true;
	Enable();
		
	
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

/*struct SceneConstantBuffer
{
	GeoMatrix projection_transform;
	GeoMatrix view_transform;
	GeoFloat4 eye_position;
	float viewport_width;
	float viewport_height;
	int clipping_enabled;
	GeoFloat4 clip_plane;
	float padding;
};

struct PerObjectConstantBuffer
{
	GeoMatrix world_transform;
	GeoMatrix world_transform_inverse;
	GeoFloat4 color;
	GeoFloat4 padding;
};
*/

bool D3D11Effect::SetFloat( const std::string& variable_name, float flt )
{
	if( "viewport_width" == variable_name )
		scene_constant_buffer.data.viewport_width = flt;
	else if( "viewport_height" == variable_name )
		scene_constant_buffer.data.viewport_height = flt;
	else
		return false;
	return true;
}

 bool D3D11Effect::SetInt( const std::string& variable_name, const int i )
 {
	if( "clipping_enabled" == variable_name )
		scene_constant_buffer.data.clipping_enabled = i;
	else
		return false;
	return true;
 }

bool D3D11Effect::SetFloatArray( const std::string& variable_name, XMFLOAT3& float_array )
{
	GeoVector v( float_array.x, float_array.y, float_array.z, 0 );
	return SetFloatArray( variable_name, v );
}

bool D3D11Effect::SetFloatArray( const std::string& variable_name, XMFLOAT4& float_array )
{
	GeoVector v(float_array);
	return SetFloatArray( variable_name, v );
}

bool D3D11Effect::SetFloatArray( const std::string& variable_name, std::vector<float>& float_array )
{
	return false;
	//memcpy( scene_buffer.data.blah, &float_array[0], sizeof(float)*float_array.size() );
}

bool D3D11Effect::SetFloatArray( const std::string& variable_name, GeoVector float_array )
{
	if( "eye_position" == variable_name )
		scene_constant_buffer.data.eye_position = float_array.ToGeoFloat4();
	else if( "clip_plane" == variable_name )
		scene_constant_buffer.data.clip_plane = float_array.ToGeoFloat4();
	else if( "color" == variable_name )
		per_object_constant_buffer.data.color = float_array.ToGeoFloat4();
	else
		return false;
	return true;
}

bool D3D11Effect::SetMatrix( const std::string& variable_name, XMMATRIX& matrix )
{
	GeoMatrix m(matrix);
	return SetMatrix( variable_name, m );
}

bool D3D11Effect::SetMatrix( const std::string& variable_name, XMFLOAT4X4& matrix )
{
	GeoMatrix m( matrix );
	return SetMatrix( variable_name, m );
}

 bool D3D11Effect::SetMatrix( const std::string& variable_name, GeoMatrix& matrix )
 {
	if( "ProjectionTransform" == variable_name )
		scene_constant_buffer.data.projection_transform = matrix;
	else if( "ViewTransform" == variable_name )
		scene_constant_buffer.data.view_transform = matrix;
	else if( "WorldTransform" == variable_name )
		per_object_constant_buffer.data.world_transform = matrix;
	else if( "WorldInverseTranspose" == variable_name )
		per_object_constant_buffer.data.world_transform_inverse = matrix;
	else
		return false; 
	return true;
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

	auto x = texture.GetShaderResource().GetResourceView();
	pGraphicsDevice->GetInternals().pDeviceContext->PSSetShaderResources( 0, 1, &x ); 
	return false;
	//return SetEffectVariable( variable_name, [&texture](ID3DX11EffectVariable* pEffectVariable){ 
		//pEffectVariable->AsShaderResource()->SetResource( texture.IsValid() ? texture.GetShaderResource().GetResourceView() : nullptr ); });	
}

bool D3D11Effect::RenderTechnique( const std::string& technique_name, std::function<void()> f )
{
	if( !valid )
		return false;

	programs[technique_name].Enable();

	scene_constant_buffer.Update();
	per_object_constant_buffer.Update();

	pGraphicsDevice->GetInternals().pDeviceContext->VSSetConstantBuffers(0,1, &scene_constant_buffer.buffer.pBuffer);
	pGraphicsDevice->GetInternals().pDeviceContext->PSSetConstantBuffers(0,1, &scene_constant_buffer.buffer.pBuffer);

	pGraphicsDevice->GetInternals().pDeviceContext->VSSetConstantBuffers(1,1, &per_object_constant_buffer.buffer.pBuffer);
	pGraphicsDevice->GetInternals().pDeviceContext->PSSetConstantBuffers(1,1, &per_object_constant_buffer.buffer.pBuffer);
	f();
	
	return true;
}
#endif
