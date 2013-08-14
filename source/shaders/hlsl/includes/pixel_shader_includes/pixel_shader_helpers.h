#include "../cross_shader_includes/uniforms.h"
#include "../cross_shader_includes/helpers.h"

SamplerState MeshTextureSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

Texture2D lala : register(t0);
//Texture2DArray lala_array;
int use_lala_array = 0;
//consider making these arrays to support separate scaling and offsetting in a texture array
float lala_horizontal_scale;
float lala_vertical_scale;
float lala_horizontal_offset;
float lala_vertical_offset;
float lala_width;
float lala_height;


float4 GetColorSample( float2 uv, float i )
{
	//if( use_lala_array )
		return lala.Sample(MeshTextureSampler, float3( Scale( uv.xy, lala_horizontal_scale, lala_vertical_scale ) + float2(lala_horizontal_offset, lala_vertical_offset), i) );
	//else
	//	return lala.Sample(MeshTextureSampler, Scale( uv.xy, lala_horizontal_scale, lala_vertical_scale ) + float2(lala_horizontal_offset, lala_vertical_offset) );
}
