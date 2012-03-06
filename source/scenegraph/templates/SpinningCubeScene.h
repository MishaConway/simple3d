#pragma once

#include "../Scene.h"
#include "SpinningCubeScene.h"

class SpinningCubeScene : public Scene
{
public:
	SpinningCubeScene();
	SpinningCubeScene( HWND hWnd, const unsigned int width, const unsigned int height, const float fovy, const float near_z, const float far_z ); 
	//virtual bool Update( const float elapsed_seconds );	
	virtual void HandleMouseMove( const unsigned int x, const unsigned int y ); 
	virtual void HandleLeftMouseDown( const unsigned int x, const unsigned int y ); 
    virtual void HandleLeftMouseUp( const unsigned int x, const unsigned int y ); 
protected:
	//virtual void RenderScene( const bool reflection = false );
    
    void ComputeSpin( const int x, const int y, const bool mouseup );
    
    void SnapCubeToAxis();
    
    void PickTiles( const unsigned int x, const unsigned int y );
    
protected:
    RenderableObject* cube;
    
    std::vector<RenderableObject*> tiles;
    RenderableObject* focused_tile;
    
    Texture red_tex;
    Texture tile_tex;
    Texture o_tex, x_tex;
    
    
    int last_mousedown_x, last_mousedown_y;
    
    std::vector<GeoVector> movements;
    
    GeoVector last_rotation_axis;
};



