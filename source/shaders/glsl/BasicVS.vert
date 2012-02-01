void main() 
{
    vec4 worldPosition, viewPosition, homogenous_screenspace_position;
	vec3 worldNormal, worldEyeVec;	
	float clip;	
	TransformPosition(  in_position, worldPosition, viewPosition, homogenous_screenspace_position, worldEyeVec, clip );
	TransformNormal( in_normal, worldNormal );
	CreateVertexShaderOut( homogenous_screenspace_position, worldNormal, worldEyeVec, in_color_uv, clip );
}