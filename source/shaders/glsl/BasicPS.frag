#include "includes/cross_shader_includes/uniforms.h"
#include "includes/cross_shader_includes/varying.h"
#include "includes/cross_shader_includes/helpers.h"
#include "includes/fragment_shader_includes/textures.h"

void main(void)
{
   // gl_FragColor = vec4( 1, 0, 0, 1 );
    //gl_FragColor = vec4( cc.g, cc.g, cc.g, 1 );
  //  gl_FragColor = color;
    gl_FragColor =  color * GetColorSample(out_color_uv,0.0); 
}