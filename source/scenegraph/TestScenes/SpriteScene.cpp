#include "SpriteScene.h"

SpriteScene::SpriteScene(){}
SpriteScene::SpriteScene( HWND hWnd, const unsigned int width, const unsigned int height, const float fovy, const float near_z, const float far_z ) : Scene( hWnd, width, height, fovy, near_z, far_z )
{
	SetBackgroundColor( Color::Blue() );
	scene_objects.push_back( new Sprite( Texture( GetRootAssetsPath() + "metalgrate.jpg" ), GeoFloat2(0,0), GeoFloat2( 0.3f, 0.3f ) ) );
}