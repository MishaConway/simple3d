#include "cross_shader_includes/uniforms.h"
#include "cross_shader_includes/varying.h"
#include "cross_shader_includes/helpers.h"
#include "includes/fragment_shader_includes/textures.h"

uniform float BlurWeights[9];

void main(void)
{
    float texel_size = 1.0 / lala_width;
	vec4 color = vec4(0,0,0,0);
	int j = 0;

	for (int i = -4; i <= 4; i++) 
	{
       color += GetColorSample( out_color_uv + vec2( texel_size*float(i), 0.0), 0.0) * BlurWeights[j];
	   j++;
	}
	gl_FragColor = vec4(color.rgb, 1.0 );
}