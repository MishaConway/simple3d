vec2 Scale( vec2 vec, float horizontal_scale, float vertical_scale )
{
	return vec2( vec.x * horizontal_scale, vec.y * vertical_scale );
}

vec2 InvertY( vec2 vec )
{
	vec.y *= -1.0;
	return vec;
}

vec3 InvertY( vec3 vec )
{
	vec.y *= -1.0;
	return vec;
}

vec4 InvertY( vec4 vec )
{
	vec.y *= -1.0;
	return vec;
}
