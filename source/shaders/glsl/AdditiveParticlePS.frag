void main(void)
{
    vec4 color_sample = GetColorSample( out_color_uv, 0.0 ); 
	float dot_factor =  abs( dot(out_normal, out_worldposition_to_eye_position ) );
	gl_FragColor = color_sample * color_sample.a * dot_factor * dot_factor * dot_factor;
}