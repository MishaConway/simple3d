//
//  ChessScene.cpp
//  Simple3D_iOS
//
//  Created by Misha Conway on 2/13/13.
//
//

#include "ChessScene.h"
#include "../../string/string_utils.h"
#include "../../system/FileIO.h"

ChessScene::ChessScene(){}
ChessScene::ChessScene( HWND hWnd, const unsigned int width, const unsigned int height, const float fovy, const float near_z, const float far_z, const std::string& root_shader_path, const std::string& root_assets_path ) : Scene( hWnd, width, height, fovy, near_z, far_z, root_shader_path, root_assets_path )
{
    last_mousedown_x = 0;
    last_mousedown_y = 0;
    movements_in_touch = 0;
    
    SetBackgroundColor( Color::Green() );
    
    const float board_width = 18;
    
    const float tile_width = board_width / 8.0f;
    
    Geometry pawn_geometry = GeometryFactory().FromSimpleModel(GetRootAssetsPath() + "correct_pawn_subdivided_once.simple" ).UniformScale(1.2f);
    
    unsigned int counter = 0;
    Geometry board_geometry;
    for( int i = 0; i < 8; i++, counter++ )
    for( int j = 0; j < 8; j++, counter++ )
    {    
        GeoVector left_back_corner = GeoVector( -tile_width*4 + tile_width/2, 0, -tile_width*4 + tile_width/2 );
        GeoVector position = left_back_corner + GeoVector( tile_width*i, 0, tile_width*j );
        
        if( j >= 6 || j <= 1 )
        {
            RenderableObject* pawn = new RenderableObject( Texture(GetRootAssetsPath() + "metalgrate.jpg" ), pawn_geometry );
            scene_objects.push_back( pawn );
            pawn->Translate( position + GeoVector(0,pawn->GetHeight()/2,0) );
            
            if( j <= 1 )
                pawn->SetColor(Color::Brown());
            else
                pawn->SetColor(Color::Beige());
            pawn->SetRotationalVelocity( GeoVector(0,1,0), 20 );
             if( j == 6 && i == 4 )
                 pawn->SetColor(Color::Red());
        }
        
        board_geometry += GeometryFactory().GenerateXZUnitSquare().Scale(tile_width,1,tile_width).Translate( position.x, 0, position.z );
    }
    
    RenderableObject* board = new RenderableObject( Texture(GetRootAssetsPath() + "metalgrate.jpg" ),board_geometry );
    board->SetPlanarReflector(true);
    scene_objects.push_back(board);
    
   // RenderableObject* floor = new RenderableObject( Texture(GetRootAssetsPath() + "metalgrate.jpg" ), //GeometryFactory().GenerateUnitCube().Scale(3,0.1f,board_width) );
    
    //scene_objects.push_back(floor);
    
    
    
    //idea for ideal camera targetting....
    
    //1) determine eye position
    GeoVector eye_position = GeoVector(0,10,board_width/2 + 1);
    
    //2) calc bottom_vec = (0, 0, board_width/2) - eye_position
    GeoVector bottom_vec = GeoVector(0,0, board_width/2.0f) - eye_position;
    
    
    
    //3) forward_vec = bottom_vec.Normalize() + GeoVec( 0, sin(fovy/2), cos( fovy/2) )
    GeoVector forward_vec = bottom_vec.Normalize() + GeoVector( 0, sin( GeoConvertToRadians(fovy/2.0f) ), -cos(GeoConvertToRadians(fovy/2.0f) ) );
    
    //4) focus_position = intersection on floor plane with vector of p1 = eye position, p2 = eye position + forward_vec
    GeoVector focus_position;
    
    GeoVector::IntersectionWithPlane(eye_position, eye_position + forward_vec, GeoVector(0,1,0), GeoVector(), &focus_position);
    
    printf( "focus position now %f, %f, %f\n", focus_position.x, focus_position.y, focus_position.z );
    eye_position.z += 6.5f;
    focus_position.z += 6.5f;
   
    camera.SetEyePosition( eye_position );
    camera.SetFocusPosition(focus_position);
    
    perform_prerendering = true;
}

void ChessScene::HandleLeftMouseDown( const unsigned int x, const unsigned int y )
{
    printf( "got touch down at %i, %i\n", x, y );
    movements_in_touch = 0;
    last_mousedown_x = x;
    last_mousedown_y = y;
 
}

void ChessScene::HandleLeftMouseUp( const unsigned int x, const unsigned int y )
{
    movements_in_touch = 0;
}

void ChessScene::HandleMouseMove( const unsigned int x, const unsigned int y )
{
    //only process if move position has changed since last time
    if( x == last_mousedown_x && y == last_mousedown_y )
        return;
    //to keep the rotations from being too sensitive, only start processing them once we've accumulated 4 movements
    if( ++movements_in_touch < 4 )
        return;
    
    //compute the rotation axis and amount of rotation and rotate each tile in worldspace
    GeoVector swipe_direction = GeoVector( (float)((int)x - last_mousedown_x), (float)((int)y - last_mousedown_y) );
    swipe_direction.x = 0;
    if( swipe_direction.y < 1 )
        swipe_direction.y = 1;
    
    GeoVector rotation_axis = swipe_direction.InvertXY().Normalize();
    
   
    
    const float swipe_amount = swipe_direction.Length();
    
   // printf( "swipe amount is %f\n", swipe_amount );
    
    
    GeoVector point_on_plane(0,2,0);
    GeoVector plane_normal(0,1,0);
    
    GeoVector p1( 10, 20, 0 );
    GeoVector p2( 10, -20, 0 );
    
    GeoVector intersection;
    
    GeoVector::IntersectionWithPlane(p1, p2, plane_normal, point_on_plane, &intersection);
    
    printf( "intersection is %f,%f,%f\n", intersection.x, intersection.y, intersection.z);
    
    
    
    
    
    //printf( "calling rotate...");
    
   // camera.Rotate(rotation_axis, swipe_amount );
    
    
   // for( unsigned int i = 0; i < scene_objects.size(); i++ )
     //   scene_objects[i]->RotateInWorldspace( rotation_axis, swipe_direction.Length()*3 );
    
    //after rotations the orientation of the tiles with respect to the camera has changed so resort them from furthest to closest
    //SortSceneObjects();
    
    //finally store the last move position
    last_mousedown_x = x;
    last_mousedown_y = y;
}
