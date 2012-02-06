#include "includes/cross_shader_includes/uniforms.h"
#include "includes/vertex_shader_includes/attributes.h"
#include "includes/cross_shader_includes/varying.h"
#include "includes/cross_shader_includes/helpers.h"
#include "includes/vertex_shader_includes/vertex_shader_helpers.h"


void main() 
{
	CreateVertexShaderOut( WorldTransform * vec4(in_position,1.0), in_normal, in_normal, InvertY(in_color_uv), 1.0 ); 
}