#pragma once

#include "../Scene.h"

class ChessScene : public Scene
{
public:
	ChessScene();
	ChessScene( HWND hWnd, const unsigned int width, const unsigned int height, const float fovy, const float near_z, const float far_z, const std::string& root_shader_path, const std::string& root_assets_path );
    
    virtual void HandleMouseMove( const unsigned int x, const unsigned int y );
	virtual void HandleLeftMouseDown( const unsigned int x, const unsigned int y );
    virtual void HandleLeftMouseUp( const unsigned int x, const unsigned int y );
    
private:
    
    int last_mousedown_x, last_mousedown_y;
    unsigned int movements_in_touch;
    
    //LoadSimpleModel( const std::string& filepath );
    
    
};
