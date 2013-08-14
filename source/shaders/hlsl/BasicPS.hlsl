#include "includes/pixel_shader_includes/pixel_shader_helpers.h"

float4 main( VertexShaderOutput input ) : SV_TARGET
{	  
	return float4(1,1,1,1);
	//return GetColorSample( input.color_uv, input.color_index ); 
	//return lala.Sample(MeshTextureSampler, float3(Scale( input.color_uv, lala_horizontal_scale, lala_vertical_scale ) + float2(lala_horizontal_offset, lala_vertical_offset), 2) );
}