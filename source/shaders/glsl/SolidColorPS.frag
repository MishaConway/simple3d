#include "cross_shader_includes/uniforms.h"
#include "cross_shader_includes/varying.h"
#include "cross_shader_includes/helpers.h"
#include "includes/fragment_shader_includes/textures.h"

uniform vec4 solid_color;
void main(void)
{
    gl_FragColor =  vec4( solid_color.rgb, solid_color.a * GetColorSample( out_color_uv, 0.0 ).a );
}