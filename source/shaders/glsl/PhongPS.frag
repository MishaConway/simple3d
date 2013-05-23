#include "includes/cross_shader_includes/uniforms.h"
#include "includes/cross_shader_includes/varying.h"
#include "includes/cross_shader_includes/helpers.h"
#include "includes/fragment_shader_includes/textures.h"
#include "includes/fragment_shader_includes/fragment_shader_helpers.h"

void main(void)
{
    
    vec3 light_vec = normalize(light_source.xyz - out_worldspace_position.xyz );
    
    float diffuse_factor = dot( light_vec, out_normal );
    

    
    // Using Blinn half angle modification for perofrmance over correctness
    vec3 h = normalize(normalize(eye_position - out_worldspace_position.xyz) + light_vec);
    float specular_factor = pow(saturate(dot(h, out_normal)), specular_power);
    
    gl_FragColor = vec4(saturate(
    (color.xyz * diffuse_factor ) + // Use light diffuse vector as intensity multiplier
    (specular_color.xyz * specular_factor * 0.5 ) // Use light specular vector as intensity multiplier
    ), 1);
    

    
    
}