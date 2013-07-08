#include "SimpleSceneUsingBasicShaderTechnique.h"

SimpleSceneUsingBasicShaderTechnique::SimpleSceneUsingBasicShaderTechnique(){}
SimpleSceneUsingBasicShaderTechnique::SimpleSceneUsingBasicShaderTechnique( WINDOW_TYPE hWnd, const unsigned int width, const unsigned int height, const float fovy, const float near_z, const float far_z, const std::string& root_shader_path, const std::string& root_assets_path ) : Scene( hWnd, width, height, fovy, near_z, far_z, root_shader_path, root_assets_path )
{
	SetBackgroundColor( Color::Green() );
	RenderableObject* cube = new RenderableObject( Texture(GetRootAssetsPath() + "metalgrate.jpg" ), GeometryFactory().GenerateUnitCube().UniformScale(2) );
	cube->SetRotationalVelocity( GeoVector(0,1,0), 20 );
	scene_objects.push_back( cube );
}