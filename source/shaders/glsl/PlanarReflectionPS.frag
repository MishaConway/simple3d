#include "includes/cross_shader_includes/uniforms.h"
#include "includes/cross_shader_includes/varying.h"
#include "includes/cross_shader_includes/helpers.h"
#include "includes/fragment_shader_includes/textures.h"

void main(void)
{
    vec2 projcoord= out_project.xy / out_project.z;
	vec4 color_sample = GetColorSample( out_color_uv, 0.0 );
	vec4 reflection_sample = GetReflectionSample(projcoord); 
	//gl_FragColor = reflection_sample;
    //gl_FragColor = vec4( color_sample.rgb * reflection_sample.rgb, 1.0 );
    float s = 0.4;
    gl_FragColor = vec4( color_sample.rgb * s + reflection_sample.rgb * (1.0-s), 1.0 );
}