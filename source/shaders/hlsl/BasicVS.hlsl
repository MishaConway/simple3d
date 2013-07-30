#include "includes/vertex_shader_includes/vertex_shader_helpers.h"


VertexShaderOutput main( VertexShaderInput input )
{
	float4 worldPosition, viewPosition, homogenous_screenspace_position;
	float3 worldNormal, worldEyeVec;	
	float clip;	
	TransformPosition( input.position, worldPosition, viewPosition, homogenous_screenspace_position, worldEyeVec, clip );
	TransformNormal( input.normal, worldNormal );
	return CreateVertexShaderOut( homogenous_screenspace_position, worldNormal, worldEyeVec, input.color_uv, clip );
}