uniform sampler2D lala;
uniform sampler2D lala1;
uniform sampler2D lala2;
uniform sampler2D lala3;
uniform float lala_horizontal_scale;
uniform float lala_vertical_scale;
uniform float lala_horizontal_offset;
uniform float lala_vertical_offset;
uniform float lala_width;
uniform float lala_height;


uniform sampler2D glowmap;
uniform float glowmap_horizontal_scale;
uniform float glowmap_vertical_scale;
uniform float glowmap_horizontal_offset;
uniform float glowmap_vertical_offset;
uniform float glowmap_width;
uniform float glowmap_height;
uniform int   glowmap_enabled;

uniform sampler2D reflection;
uniform float reflection_horizontal_scale;
uniform float reflection_vertical_scale;
uniform float reflection_horizontal_offset;
uniform float reflection_vertical_offset;
uniform float reflection_width;
uniform float reflection_height;

vec4 GetColorSample( vec2 uv, float i )
{
	return texture2D(lala, Scale( uv.xy, lala_horizontal_scale, lala_vertical_scale ) + vec2(lala_horizontal_offset, lala_vertical_offset));
}

vec4 GetGlowSample( vec2 uv )
{
	if( 1 == glowmap_enabled )
		return texture2D( glowmap, Scale( uv.xy, glowmap_horizontal_scale, glowmap_vertical_scale ) + vec2(glowmap_horizontal_offset, glowmap_vertical_offset) );
	else
		return vec4(0,0,0,0);	
}

vec4 GetReflectionSample( vec2 uv )
{
	//return texture2D( reflection, uv.xy );
	return texture2D( reflection, Scale( uv.xy, reflection_horizontal_scale, reflection_vertical_scale ) + vec2(reflection_horizontal_offset, reflection_vertical_offset) );	
}