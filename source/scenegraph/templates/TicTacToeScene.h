#pragma once

#include "../Scene.h"
//#include "SpinningCubeScene.h"

struct TicTacToeMove
{
    unsigned int x, y, z;
    char value;
};

class TicTacToeScene : public Scene
{
public:
	TicTacToeScene();
	TicTacToeScene( HWND hWnd, const unsigned int width, const unsigned int height, const float fovy, const float near_z, const float far_z, const char x_or_o_player ); 
    
    virtual bool Update( const float elapsed_seconds );

	virtual void HandleMouseMove( const unsigned int x, const unsigned int y ); 
	virtual void HandleLeftMouseDown( const unsigned int x, const unsigned int y ); 
    virtual void HandleLeftMouseUp( const unsigned int x, const unsigned int y );
    
    void PlayMove( TicTacToeMove move );
    
    void ReenablePlay();
    bool HasSelectedMove();
    TicTacToeMove GetSelectedMove();
    void SubmitSelectedMove();
protected:
    void PickTiles( const unsigned int x, const unsigned int y );
    void SortSceneObjects();
    RenderableObject* GetTile( const unsigned int x, const unsigned int y, const unsigned z );
protected:    
    std::vector<RenderableObject*> tiles;
    RenderableObject* focused_tile;
    Texture tile_tex, o_tex, x_tex;
    int last_mousedown_x, last_mousedown_y;
    unsigned int movements_in_touch;
    
    char x_or_o_player;
    bool playing_enabled;
    bool has_selected_move;
    TicTacToeMove selected_move;
};



