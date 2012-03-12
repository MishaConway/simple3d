#include "TicTacToeScene.h"
#include <math.h>
#include <algorithm>
#include "../../string/string_utils.h"
#include "../../std/nullptr.h"

TicTacToeScene::TicTacToeScene(){}
TicTacToeScene::TicTacToeScene( HWND hWnd, const unsigned int width, const unsigned int height, const float fovy, const float near_z, const float far_z, const char x_or_o_player  ) : Scene( hWnd, width, height, fovy, near_z, far_z )
{
	SetBackgroundColor( Color::MidnightBlue());

    //set x_or_o_player
    this->x_or_o_player = tolower(x_or_o_player);
    playing_enabled = true;
    
    //initialize various values to zero or null state
    focused_tile = nullptr; 
    last_mousedown_x = 0;
    last_mousedown_y = 0;
    movements_in_touch = 0;
    
    //create all textures used for the board
    Color tile_tex_color = Color::White();
    tile_tex_color.a = 90;
    
    Color x_text_color = Color::Green();
    Color o_text_color = Color::Orange();
    Color temp_x_text_color = Color::Gray();
    Color temp_o_text_color = Color::Gray();
    
    
    tile_tex = Texture( 32, 32, tile_tex_color );
    o_tex = Texture::FromText("O", o_text_color, tile_tex_color);
    x_tex = Texture::FromText("X", x_text_color, tile_tex_color);
    temp_o_tex = Texture::FromText("O", temp_x_text_color, tile_tex_color);
    temp_x_tex = Texture::FromText("X", temp_o_text_color, tile_tex_color);
    
    //configure distances used to build board
    const unsigned int num_tiles_per_row = 3;
    const unsigned int num_tiles_per_col = 3;
    const float separation = 0.93f;
    const float slice_size = 1.5f;
    const float slice_width = slice_size;
    const float slice_height = slice_size;
    const float tile_width = slice_width / (float)num_tiles_per_row;
    const float tile_height = slice_height / (float)num_tiles_per_col;
    const float tile_spacing = 0.03f;
       
    //construct all tile renderable objects
    for( int z = 0; z < 3; z++ )
    for( int x = 0; x < 3; x++ )
    for( int y = 0; y < 3; y++ )
    {
        RenderableObject* tile = new RenderableObject( tile_tex, GeometryFactory().GenerateXYUnitSquare().Scale(tile_width, tile_height, 1 ));
        tile->Translate( -slice_width / 2.0f + tile_width * x + x * tile_spacing, -slice_height / 2.0f + tile_height * y + y * tile_spacing,  separation - z * separation );
        tile->SetBlendType(BlendType::ALPHA);
        tile->SetTwoSided(true);
        tile->SetUserData( "X", IntToString(x) );
        tile->SetUserData( "Y", IntToString(y) );
        tile->SetUserData( "Z", IntToString(z) );
        
        scene_objects.push_back(tile);
        tiles.push_back(tile);
    }
    
    //construct UI objects
    background = new Sprite( Texture( "assets/space_backgrounds/1.jpg" ), GeoFloat2(0,0), GeoFloat2( 2, 2 ) );
    
    //sprites.push_back(background);
        
    //finally we want to start the scene with all objects sorted from most distant to closest to camera
    SortSceneObjects();
}

void TicTacToeScene::RenderScene( const bool reflection )
{
    background->Render();
    Scene::RenderScene( reflection );
}

bool TicTacToeScene::Update( const float elapsed_seconds )
{
    for( unsigned int i = 0; i < tiles.size(); i++ )
        if( tiles[i]->GetUserData("value").empty() )
            tiles[i]->SetTexture(tile_tex);
        else if( "x" == tiles[i]->GetUserData("value") )
            tiles[i]->SetTexture(temp_x_tex);
        else if( "X" == tiles[i]->GetUserData("value") )
            tiles[i]->SetTexture(x_tex);
        else if( "o" == tiles[i]->GetUserData("value") )
            tiles[i]->SetTexture(temp_o_tex);
        else if( "O" == tiles[i]->GetUserData("value") )
            tiles[i]->SetTexture(o_tex);
    
    float horizontal_offset = background->GetTexture().GetHorizontalOffset();
    float vertical_offset = background->GetTexture().GetVerticalOffset();
    horizontal_offset += 0.03f * elapsed_seconds;
    
    background->GetTexture().SetOffsets( horizontal_offset, vertical_offset );
    
    return Scene::Update(elapsed_seconds);
}

void TicTacToeScene::HandleLeftMouseDown( const unsigned int x, const unsigned int y )
{
    movements_in_touch = 0;
    last_mousedown_x = x;
    last_mousedown_y = y;
    if( playing_enabled )
        PickTiles( x, y );
}

void TicTacToeScene::HandleLeftMouseUp( const unsigned int x, const unsigned int y )
{
    movements_in_touch = 0;
}

void TicTacToeScene::HandleMouseMove( const unsigned int x, const unsigned int y )
{    
    //only process if move position has changed since last time
    if( x == last_mousedown_x && y == last_mousedown_y )
        return;
    //to keep the rotations from being too sensitive, only start processing them once we've accumulated 4 movements
    if( ++movements_in_touch < 4 )
        return;
    
    //compute the rotation axis and amount of rotation and rotate each tile in worldspace
    GeoVector swipe_direction = GeoVector( (int)x - last_mousedown_x, (int)y - last_mousedown_y );    
    GeoVector rotation_axis = swipe_direction.InvertXY().Normalize();
    for( unsigned int i = 0; i < tiles.size(); i++ )
        tiles[i]->RotateInWorldspace( rotation_axis, swipe_direction.Length()*3 );
    
    //after rotations the orientation of the tiles with respect to the camera has changed so resort them from furthest to closest
    SortSceneObjects();
    
    //finally store the last move position
    last_mousedown_x = x;
    last_mousedown_y = y;  
}

void TicTacToeScene::SortSceneObjects()
{
    struct RenderableObjectDistanceNode
    {
        RenderableObject* renderable_object;
        float distance_from_cam;
        bool operator < (const RenderableObjectDistanceNode& r) const
        {
            return (distance_from_cam < r.distance_from_cam);
        }
    };
    
    std::map< RenderableObject*, float > objects_with_distance_from_cam;
    std::vector<RenderableObjectDistanceNode> renderable_object_distance_nodes;
    for( unsigned int i = 0; i < scene_objects.size(); i++ )
    {
        RenderableObjectDistanceNode r;
        r.renderable_object = scene_objects[i];
        r.distance_from_cam = camera.DistanceFromEye(scene_objects[i]->GetWorldspaceCentroid() );
        renderable_object_distance_nodes.push_back(r);
    }
    
    std::sort( renderable_object_distance_nodes.begin(), renderable_object_distance_nodes.end() );
    std::reverse( renderable_object_distance_nodes.begin(), renderable_object_distance_nodes.end() );
    scene_objects.clear();
    for( unsigned int i = 0; i < renderable_object_distance_nodes.size(); i++ )
        scene_objects.push_back(renderable_object_distance_nodes[i].renderable_object);    
}

void TicTacToeScene::PickTiles( const unsigned int x, const unsigned int y )
{
    std::vector<RenderableObject*> selected_tile_candidates;
    
    for( unsigned int i = 0; i < tiles.size(); i++ )
    {
        std::vector<Vertex> worldspace_vertices = tiles[i]->GetWorldspaceVertices();
        std::vector<GeoVector> screenspace_vertices;
        for( unsigned int j = 0; j < worldspace_vertices.size(); j++ )
            screenspace_vertices.push_back( camera.ProjectIntoScreenspace( GeoVector(worldspace_vertices[j].position)) );
        bool selected = false;
        for( unsigned int j = 0; j < screenspace_vertices.size(); j+= 3 )
        {
            GeoTriangle screenspace_triangle( screenspace_vertices[j], screenspace_vertices[j+1], screenspace_vertices[j+2]);
            selected = selected || screenspace_triangle.PointInsideXYTriangle( GeoFloat2(x, y) );
        }
        
        if( selected )
            selected_tile_candidates.push_back(tiles[i]);
    }
    
    if( selected_tile_candidates.empty() )
        focused_tile = 0;
    else 
    {
        focused_tile = selected_tile_candidates[0];
        float min_distance_from_eye = camera.DistanceFromEye( selected_tile_candidates[0]->GetWorldspaceCentroid() );
        for( unsigned int i = 1; i < selected_tile_candidates.size(); i++ )
        {
            if( camera.DistanceFromEye( selected_tile_candidates[i]->GetWorldspaceCentroid() ) < min_distance_from_eye )
            {
                min_distance_from_eye = camera.DistanceFromEye( selected_tile_candidates[i]->GetWorldspaceCentroid() );
                focused_tile = selected_tile_candidates[i];
            }        
        }
        
        if( focused_tile->GetUserData( "value" ).empty() )
        {
            for( unsigned int i = 0; i < tiles.size(); i++ )
                if( "x" == tiles[i]->GetUserData("value") || "o" == tiles[i]->GetUserData("value") )
                    tiles[i]->SetUserData("value", "" );
            
            focused_tile->SetUserData("value", CharToString(x_or_o_player) );
            has_selected_move = true;
            selected_move.x = atoi( focused_tile->GetUserData("X").c_str() );
            selected_move.y = atoi( focused_tile->GetUserData("Y").c_str() );
            selected_move.z = atoi( focused_tile->GetUserData("Z").c_str() );
            selected_move.value = x_or_o_player;
        }
    }
}

RenderableObject* TicTacToeScene::GetTile( const unsigned int x, const unsigned int y, const unsigned z )
{
    for( unsigned int i = 0; i < tiles.size(); i++ )
    {
        const unsigned int X = atoi( tiles[i]->GetUserData("X").c_str() ); 
        const unsigned int Y = atoi( tiles[i]->GetUserData("Y").c_str() ); 
        unsigned int Z = atoi( tiles[i]->GetUserData("Z").c_str() ); 
        if( x == X && y == Y && z == Z )
            return tiles[i];
    }
    
    return nullptr;
}

void TicTacToeScene::PlayMove( TicTacToeTile move )
{
    GetTile( move.x, move.y, move.z )->SetUserData("value", CharToString(toupper(move.value)) );
}

void TicTacToeScene::PlayRandomMove(const char value)
{
    std::vector<RenderableObject*> available_tiles;
    for( unsigned int i = 0; i < tiles.size(); i++ )
        if( tiles[i]->GetUserData("value").empty() )
            available_tiles.push_back(tiles[i]);
    
    if( !available_tiles.empty() )
    {
        std::random_shuffle(available_tiles.begin(), available_tiles.end() );
        available_tiles.front()->SetUserData("value", CharToString(toupper(value)) );
    }
}

void TicTacToeScene::EnablePlaying()
{
    playing_enabled = true;
}

void TicTacToeScene::DisablePlaying()
{
    playing_enabled = false;
}

bool TicTacToeScene::IsPlayingEnabled()
{
    return playing_enabled;
}

bool TicTacToeScene::HasSelectedMove()
{
    return playing_enabled && has_selected_move;
}

TicTacToeTile TicTacToeScene::GetSelectedMove()
{
    return selected_move;
}

void TicTacToeScene::SubmitSelectedMove()
{
    PlayMove( selected_move );
    playing_enabled = false;
    has_selected_move = false;
}




