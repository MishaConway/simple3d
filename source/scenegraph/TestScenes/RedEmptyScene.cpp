#include "RedEmptyScene.h"

RedEmptyScene::RedEmptyScene(){}
RedEmptyScene::RedEmptyScene( WINDOW_TYPE hWnd, const unsigned int width, const unsigned int height, const float fovy, const float near_z, const float far_z, const std::string& root_shader_path, const std::string& root_assets_path ) : Scene( hWnd, width, height, fovy, near_z, far_z, root_shader_path, root_assets_path )
{
	SetBackgroundColor( Color::Red() );
}