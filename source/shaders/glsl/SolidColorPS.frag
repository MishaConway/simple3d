uniform vec4 solid_color;
void main(void)
{
    gl_FragColor =  vec4( solid_color.rgb, solid_color.a * GetColorSample( out_color_uv, 0.0 ).a );
}