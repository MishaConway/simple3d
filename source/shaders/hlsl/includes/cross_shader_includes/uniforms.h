cbuffer scene
{
float4x4 ViewTransform;
float4x4 ProjectionTransform;
};

cbuffer object
{
float4x4 WorldTransform;
float4x4 WorldInverseTranspose;

float4 color;

int clipping_enabled = 0;
float4 clip_plane = float4( 0, 1, 0, 1.0f );


float3 eye_position;

float viewport_width;
float viewport_height;
};




struct VertexShaderOutput
{
    float4 position : SV_POSITION;
	linear float2 color_uv : TEXCOORD0;
	linear float3 normal   : TEXCOORD1;
	linear float3 worldposition_to_eye_position : TEXCOORD2;
	/*nointerpolation*/ float color_index : TEXCOORD3;
	linear float3 project  : TEXCOORD4;
	linear float  clip		: TEXCOORD5;
};