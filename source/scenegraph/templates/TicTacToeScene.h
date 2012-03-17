#pragma once

#include "../Scene.h"

struct TicTacToeTile
{
    unsigned int x, y, z;
    char value;
};

class TicTacToeScene : public Scene
{
public:
	TicTacToeScene();
	TicTacToeScene( HWND hWnd, const unsigned int width, const unsigned int height, const float fovy, const float near_z, const float far_z, const char x_or_o_player ); 
    
    virtual void RenderScene( const bool reflection = false );
    virtual bool Update( const float elapsed_seconds );

	virtual void HandleMouseMove( const unsigned int x, const unsigned int y ); 
	virtual void HandleLeftMouseDown( const unsigned int x, const unsigned int y ); 
    virtual void HandleLeftMouseUp( const unsigned int x, const unsigned int y );
    
    void PlayMove( TicTacToeTile move );
    void PlayRandomMove(const char value);
    
    
    
    void EnablePlaying();
    void DisablePlaying();
    bool IsPlayingEnabled();
    bool HasSelectedMove();
    TicTacToeTile GetSelectedMove();
    void SubmitSelectedMove();
protected:
    void PickTiles( const unsigned int x, const unsigned int y );
    void SortSceneObjects();
    RenderableObject* GetTile( const unsigned int x, const unsigned int y, const unsigned z );
protected:    
    std::vector<RenderableObject*> tiles;
    RenderableObject* focused_tile;
    std::vector<RenderableObject*> stars;
    Texture tile_tex, o_tex, x_tex, temp_o_tex, temp_x_tex;
    int last_mousedown_x, last_mousedown_y;
    unsigned int movements_in_touch;
    
    char x_or_o_player;
    bool playing_enabled;
    bool has_selected_move;
    TicTacToeTile selected_move;
    
    Sprite* background;
};



