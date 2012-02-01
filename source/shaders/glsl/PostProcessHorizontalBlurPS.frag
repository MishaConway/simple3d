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