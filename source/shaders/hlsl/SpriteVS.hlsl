#include "includes/vertex_shader_includes/vertex_shader_helpers.h"

VertexShaderOutput  main( VertexShaderInput input )
{
	return CreateVertexShaderOut( mul(float4(input.position,1.0), WorldTransform), input.normal, input.normal, input.color_uv, 1 );      
}