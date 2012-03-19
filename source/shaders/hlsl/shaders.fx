
struct VertexShaderInput
{
    float3 position  : POSITION;
	float3 color_uv  : COLOR_UV;
	float3 normal    : NORMAL;
	float3 tangent   : TANGENT;
	float3 bitangent : BITANGENT;
};

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
	float2 color_uv : COLOR_UV;
	float3 normal   : NORMAL;
	float3 worldposition_to_eye_position : EYE;
	nointerpolation float color_index : COLOR_INDEX;
	float3 project  : PROJECTION;
	float  clip		: SV_ClipDistance;
};

float4x4 WorldTransform;
float4x4 ViewTransform;
float4x4 ProjectionTransform;
float4x4 WorldInverseTranspose;

float4 color;

int clipping_enabled = 0;
float4 clip_plane = float4( 0, 1, 0, 1.0f );


float3 eye_position;

float viewport_width;
float viewport_height;

Texture2DArray lala;
Texture2DArray lala_array;
int use_lala_array = 0;
//consider making these arrays to support separate scaling and offsetting in a texture array
float lala_horizontal_scale;
float lala_vertical_scale;
float lala_horizontal_offset;
float lala_vertical_offset;
float lala_width;
float lala_height;

Texture2D reflection;
float reflection_horizontal_scale;
float reflection_vertical_scale;
float reflection_horizontal_offset;
float reflection_vertical_offset;
float reflection_width;
float reflection_height;

Texture2D glowmap;
float glowmap_horizontal_scale;
float glowmap_vertical_scale;
float glowmap_horizontal_offset;
float glowmap_vertical_offset;
float glowmap_width;
float glowmap_height;

SamplerState MeshTextureSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

float2 Scale( float2 vec, float horizontal_scale, float vertical_scale )
{
	return float2( vec.x * horizontal_scale, vec.y * vertical_scale );
}

float2 InvertY( float2 vec )
{
	vec.y *= -1;
	return vec;
}

float3 InvertY( float3 vec )
{
	vec.y *= -1;
	return vec;
}

float4 InvertY( float4 vec )
{
	vec.y *= -1;
	return vec;
}

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

float4 GetColorSample( float2 uv, float i )
{
	//if( use_lala_array )
		return lala.Sample(MeshTextureSampler, float3( Scale( uv.xy, lala_horizontal_scale, lala_vertical_scale ) + float2(lala_horizontal_offset, lala_vertical_offset), i) );
	//else
	//	return lala.Sample(MeshTextureSampler, Scale( uv.xy, lala_horizontal_scale, lala_vertical_scale ) + float2(lala_horizontal_offset, lala_vertical_offset) );
}

VertexShaderOutput BasicVS( VertexShaderInput input )
{
	float4 worldPosition, viewPosition, homogenous_screenspace_position;
	float3 worldNormal, worldEyeVec;	
	float clip;	
	TransformPosition( input.position, worldPosition, viewPosition, homogenous_screenspace_position, worldEyeVec, clip );
	TransformNormal( input.normal, worldNormal );
	return CreateVertexShaderOut( homogenous_screenspace_position, worldNormal, worldEyeVec, input.color_uv, clip );
}

float4 BasicPS( VertexShaderOutput input ) : SV_Target
{	  
	return GetColorSample( input.color_uv, input.color_index ); 
	//return lala.Sample(MeshTextureSampler, float3(Scale( input.color_uv, lala_horizontal_scale, lala_vertical_scale ) + float2(lala_horizontal_offset, lala_vertical_offset), 2) );
}

float4 AdditiveParticlePS( VertexShaderOutput input ) : SV_Target
{	  
	float4 color_sample = GetColorSample( input.color_uv, input.color_index ); 
	float dot_factor =  abs( dot(input.normal, input.worldposition_to_eye_position ) );
	return color_sample * color_sample.a /* 0.4f*/ * dot_factor * dot_factor * dot_factor;
	//return lala.Sample(MeshTextureSampler, float3(Scale( input.color_uv, lala_horizontal_scale, lala_vertical_scale ) + float2(lala_horizontal_offset, lala_vertical_offset), 2) );
}

float BlurWeights[9];
float4 PostProcessHorizontalBlurPS( VertexShaderOutput input ) : SV_Target
{	  
	float texel_size = 1.0f / lala_width;
	float4 color = 0;
	int j = 0;

	[unroll]
	for (int i = -4; i <= 4; i++) 
	{
       color += 1.2 * GetColorSample( input.color_uv + float3( texel_size*i, 0, 0), input.color_index ) * BlurWeights[j];
	   j++;
   }
	return float4(color.rgb, 1 );
}

float4 PostProcessVerticalBlurPS( VertexShaderOutput input ) : SV_Target
{	  
	float texel_size = 1.0f / lala_height;
	float4 color = 0;
	int j = 0;
	[unroll]
	for (int i = -4; i <= 4; i++) 
	{
       color += 1.2 * GetColorSample( input.color_uv + float3( 0, texel_size*i, 0 ), input.color_index  )* BlurWeights[j];
	   j++;
   }
	return float4(color.rgb, 1 );
}

float4 GlowFillPS( VertexShaderOutput input ) : SV_Target
{	  
	float4 color_sample = GetColorSample( input.color_uv, input.color_index  );
	float4 glow_sample = glowmap.Sample(MeshTextureSampler, Scale( input.color_uv, glowmap_horizontal_scale, glowmap_vertical_scale ) + float2(glowmap_horizontal_offset, glowmap_vertical_offset) );
	return float4( color_sample.rgb * glow_sample.a, glow_sample.a );
}

float4 solid_color = float4( 0.3f, 0.3f, 0.3f, 0.6f );
float4 SolidColorPS( VertexShaderOutput input ) : SV_Target
{
	return float4( solid_color.rgb, solid_color.a * GetColorSample( input.color_uv, input.color_index  ).a );
}


float progress;
float4 ProgressSpritePS( VertexShaderOutput input ) : SV_Target
{	  
	float4 color_sample = GetColorSample( input.color_uv, input.color_index  );
	color_sample.a *= 0.6f;
	float3 progressfill_color = float3(0,1,0);

	float2 dir_from_center = normalize(input.color_uv - float2(0.5f, 0.5f));	 
	if( atan2( dir_from_center.y, dir_from_center.x ) + 3.14f  < 2.0f* 3.14f * progress )
		return float4( progressfill_color, color_sample.a);
	return color_sample;
 }

float4 LinePS( VertexShaderOutput input ) : SV_Target
{
	return float4( 0.0f, 0.0f, 0.0f, 1.0f );
}

VertexShaderOutput  SpriteVS( VertexShaderInput input )
{
	return CreateVertexShaderOut( mul(float4(input.position,1.0), WorldTransform), input.normal, input.normal, input.color_uv, 1 );      
}

float4 PlanarReflectionPS( VertexShaderOutput input ) : SV_Target
{
	float2 projcoord=(input.project.xy / input.project.z);
	float4 color_sample = GetColorSample( input.color_uv, input.color_index  );
	float4 reflection_sample = reflection.Sample(MeshTextureSampler, projcoord ); 
	return float4( color_sample.rgb * reflection_sample.rgb, 1 );
}
	 
  // Technique Definition
    technique11 Basic
    {
        pass P0
        {
            SetVertexShader( CompileShader( vs_4_0, BasicVS() ) );
            SetGeometryShader( NULL );
            SetPixelShader( CompileShader( ps_4_0, BasicPS() ) );
        }
    }

	technique11 SolidColor
    {
        pass P0
        {
            SetVertexShader( CompileShader( vs_4_0, BasicVS() ) );
            SetGeometryShader( NULL );
            SetPixelShader( CompileShader( ps_4_0, SolidColorPS() ) );
        }
    }

	technique11 GlowFill
    {
        pass P0
        {
            SetVertexShader( CompileShader( vs_4_0, BasicVS() ) );
            SetGeometryShader( NULL );
            SetPixelShader( CompileShader( ps_4_0, GlowFillPS() ) );
        }
    }

	technique11 PostProcessHorizontalBlur
    {
        pass P0
        {
            SetVertexShader( CompileShader( vs_4_0, SpriteVS() ) );
            SetGeometryShader( NULL );
            SetPixelShader( CompileShader( ps_4_0, PostProcessHorizontalBlurPS() ) );
        }
    }

	technique11 PostProcessVerticalBlur
    {
        pass P0
        {
            SetVertexShader( CompileShader( vs_4_0, SpriteVS() ) );
            SetGeometryShader( NULL );
            SetPixelShader( CompileShader( ps_4_0, PostProcessVerticalBlurPS() ) );
        }
    }

	technique11 AdditiveParticle
    {
        pass P0
        {
            SetVertexShader( CompileShader( vs_4_0, BasicVS() ) );
            SetGeometryShader( NULL );
            SetPixelShader( CompileShader( ps_4_0, AdditiveParticlePS() ) );
        }
    }

	technique11 PlanarReflection
    {
        pass P0
        {
            SetVertexShader( CompileShader( vs_4_0, BasicVS() ) );
            SetGeometryShader( NULL );
            SetPixelShader( CompileShader( ps_4_0, PlanarReflectionPS() ) );
        }
    }

	technique11 Sprite
	{
		pass P0
		{
            SetVertexShader( CompileShader( vs_4_0, SpriteVS() ) );
            SetGeometryShader( NULL );
            SetPixelShader( CompileShader( ps_4_0, BasicPS() ) );
		}
	}

	technique11 SolidColorSprite
	{
		pass P0
		{
            SetVertexShader( CompileShader( vs_4_0, SpriteVS() ) );
            SetGeometryShader( NULL );
            SetPixelShader( CompileShader( ps_4_0, SolidColorPS() ) );
		}
	}

	technique11 ProgressSprite
	{
		pass P0
		{
            SetVertexShader( CompileShader( vs_4_0, SpriteVS() ) );
            SetGeometryShader( NULL );
            SetPixelShader( CompileShader( ps_4_0, ProgressSpritePS() ) );
		}
	}

	technique11 Line
	{
		pass P0
		{
            SetVertexShader( CompileShader( vs_4_0, BasicVS() ) );
            SetGeometryShader( NULL );
            SetPixelShader( CompileShader( ps_4_0, LinePS() ) );
		}
	}