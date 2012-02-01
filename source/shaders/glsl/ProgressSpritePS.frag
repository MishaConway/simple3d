uniform float progress;

void main(void)
{
    vec4 color_sample = GetColorSample( out_color_uv, 0.0 );
	color_sample.a *= 0.6;
	vec3 progressfill_color = vec3(0,1,0);

	vec2 dir_from_center = normalize(out_color_uv - vec2(0.5, 0.5));	 
	if( atan( dir_from_center.x , dir_from_center.y ) + 3.14  < 2.0 * 3.14 * progress )
		gl_FragColor = vec4( progressfill_color, color_sample.a); 
	else
		gl_FragColor = color_sample;
}