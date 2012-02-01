void main(void)
{
    vec2 projcoord= out_project.xy / out_project.z;
	vec4 color_sample = GetColorSample( out_color_uv, 0.0 );
	vec4 reflection_sample = GetReflectionSample(projcoord); 
	gl_FragColor = vec4( color_sample.rgb * reflection_sample.rgb, 1.0 );
}