#include "cross_shader_includes/uniforms.h"
#include "cross_shader_includes/varying.h"
#include "cross_shader_includes/helpers.h"
#include "includes/fragment_shader_includes/textures.h"

void main(void)
{
    vec2 projcoord= out_project.xy / out_project.z;
	vec4 color_sample = GetColorSample( out_color_uv, 0.0 );
	vec4 reflection_sample = GetReflectionSample(projcoord); 
	gl_FragColor = vec4( color_sample.rgb * reflection_sample.rgb, 1.0 );
}