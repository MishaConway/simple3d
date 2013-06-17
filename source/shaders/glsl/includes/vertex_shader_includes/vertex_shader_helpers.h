void TransformPosition( in vec3 objectspace_position, out vec4 worldspace_position, out vec4 viewspace_position, out vec4 homogenous_screenspace_position, out vec3 worldposition_to_eye_position, out float clip )
{
	worldspace_position				= WorldTransform * vec4(objectspace_position,1.0);
	viewspace_position				= ViewTransform * worldspace_position;
	homogenous_screenspace_position = ProjectionTransform * viewspace_position;	
	//worldspace_position				= vec4(objectspace_position,1.0) * WorldTransform;
	//viewspace_position				=  worldspace_position * ViewTransform;
	//homogenous_screenspace_position = viewspace_position * ProjectionTransform ;	
	worldposition_to_eye_position	= normalize(eye_position - worldspace_position.xyz);
	if( 1 == clipping_enabled )
		clip = dot( worldspace_position, clip_plane);
	else
		clip = 1.0;
}

void TransformNormal( in vec3 objectspace_normal, out vec3 worldspace_normal )
{
	worldspace_normal = normalize(WorldInverseTranspose * vec4(objectspace_normal,0)).xyz;
	//worldspace_normal = normalize(mul(objectspace_normal, WorldTransform));
	//worldspace_normal = normalize( vec4(objectspace_normal,0) * WorldInverseTranspose).xyz;
}

void CreateVertexShaderOut( vec4 position, vec4 world_position, vec3 normal, vec3 worldposition_to_eye_position, vec3 color_uv, float clip )
{
	gl_Position = position;
    out_worldspace_position = world_position;
	out_normal = normal;
	out_worldposition_to_eye_position = worldposition_to_eye_position;
	out_color_uv = InvertY(color_uv.xy);
	if( 1 == compute_projective_texture_coordinates )
	{
		out_project = position;
        //out_project = vec4( vec2(position.x*0.5, position.y*-0.5) + vec2(0.5, -0.5), position.zw );
        
        //out_project.y *= -1.0;
        //out_project.x = 0.5 * (position.w + position.x);
		//out_project.y = 0.5 * (position.w - position.y);
		//out_project.z = position.w;
	}
	else
		out_project = vec4( 0, 0, 0, 0 );
	out_clip = clip;
	//output.color_index = color_uv.z;
}