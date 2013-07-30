#include "../cross_shader_includes/uniforms.h"
#include "../cross_shader_includes/helpers.h"

struct VertexShaderInput
{
    float3 position  : POSITION;
	float3 color_uv  : TEXCOORD0;
	float3 normal    : TEXCOORD1;
	float3 tangent   : TEXCOORD2;
	float3 bitangent : TEXCOORD3;
};

void TransformPosition( in float3 objectspace_position, out float4 worldspace_position, out float4 viewspace_position, out float4 homogenous_screenspace_position, out float3 worldposition_to_eye_position, out float clip )
{
	worldspace_position				= mul(float4(objectspace_position,1.0), WorldTransform);
	viewspace_position				= mul(worldspace_position, ViewTransform);
	homogenous_screenspace_position = mul(viewspace_position, ProjectionTransform);	
	worldposition_to_eye_position	= normalize(eye_position - worldspace_position.xyz);
	//if( clipping_enabled )
	//	clip = dot( worldspace_position, clip_plane);
	//else
		clip = 1.0f;
}

void TransformNormal( in float3 objectspace_normal, out float3 worldspace_normal )
{
	//worldspace_normal = normalize(mul(WorldInverseTranspose, float4(objectspace_normal,0)));
	//worldspace_normal = normalize(mul(objectspace_normal, WorldTransform));
	worldspace_normal = normalize(mul(float4(objectspace_normal,0), WorldInverseTranspose));
}

int compute_projective_texture_coordinates = 1;
VertexShaderOutput CreateVertexShaderOut( float4 position, float3 normal, float3 worldposition_to_eye_position, float3 color_uv, float clip )
{
	VertexShaderOutput output;
	output.position = position;
	output.normal = normal;
	output.worldposition_to_eye_position = worldposition_to_eye_position;
	output.color_uv = InvertY(color_uv.xy);
	output.color_index = color_uv.z;
	if( compute_projective_texture_coordinates )
	{
		output.project.x = 0.5 * (output.position.w + output.position.x);
		output.project.y = 0.5 * (output.position.w - output.position.y);
		output.project.z = output.position.w;
	}
	else
		output.project = float3( 0, 0, 0 );
	output.clip = clip;
	return output;	
}

