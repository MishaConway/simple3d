#include "SpriteScene.h"

SpriteScene::SpriteScene(){}
SpriteScene::SpriteScene( WINDOW_TYPE hWnd, const unsigned int width, const unsigned int height, const float fovy, const float near_z, const float far_z, const std::string& root_shader_path, const std::string& root_assets_path ) : Scene( hWnd, width, height, fovy, near_z, far_z, root_shader_path, root_assets_path )
{
	SetBackgroundColor( Color::Blue() );
	scene_objects.push_back( new Sprite( Texture( GetRootAssetsPath() + "metalgrate.jpg" ), GeoFloat2(0,0), GeoFloat2( 0.3f, 0.3f ) ) );
}