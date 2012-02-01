#include "RedEmptyScene.h"

RedEmptyScene::RedEmptyScene(){}
RedEmptyScene::RedEmptyScene( HWND hWnd, const unsigned int width, const unsigned int height, const float fovy, const float near_z, const float far_z ) : Scene( hWnd, width, height, fovy, near_z, far_z )
{
	SetBackgroundColor( Color::Red() );
}