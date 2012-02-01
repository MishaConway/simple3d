void main() 
{
	CreateVertexShaderOut( InvertY( WorldTransform * vec4(in_position,1.0)), in_normal, in_normal, InvertY(in_color_uv), 1.0 ); 
}