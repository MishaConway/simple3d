#include "SimpleSceneUsingBasicShaderTechnique.h"

SimpleSceneUsingBasicShaderTechnique::SimpleSceneUsingBasicShaderTechnique(){}
SimpleSceneUsingBasicShaderTechnique::SimpleSceneUsingBasicShaderTechnique( HWND hWnd, const unsigned int width, const unsigned int height, const float fovy, const float near_z, const float far_z ) : Scene( hWnd, width, height, fovy, near_z, far_z )
{
	SetBackgroundColor( Color::Green() );
	RenderableObject* cube = new RenderableObject( Texture(GetRootAssetsPath() + "metalgrate.jpg" ), GeometryFactory().GenerateUnitCube().UniformScale(2) );
	cube->SetRotationalVelocity( GeoVector(0,1,0), 20 );
	scene_objects.push_back( cube );
}