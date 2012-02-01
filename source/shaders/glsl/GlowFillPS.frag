void main(void)
{
    vec4 color_sample = GetColorSample( out_color_uv, 0.0 );
	vec4 glow_sample = GetGlowSample( out_color_uv );
	gl_FragColor = vec4( color_sample.rgb * glow_sample.a, glow_sample.a );
}