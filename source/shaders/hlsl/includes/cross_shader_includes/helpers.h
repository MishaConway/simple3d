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