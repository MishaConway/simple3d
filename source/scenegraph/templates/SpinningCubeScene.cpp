#include "SpinningCubeScene.h"
#include <math.h>
#include <algorithm>
#include "../../string/string_utils.h"

SpinningCubeScene::SpinningCubeScene(){}
SpinningCubeScene::SpinningCubeScene( HWND hWnd, const unsigned int width, const unsigned int height, const float fovy, const float near_z, const float far_z ) : Scene( hWnd, width, height, fovy, near_z, far_z )
{
	SetBackgroundColor( Color::Green() );
	cube = new RenderableObject( Texture( "assets/metalgrate.jpg" ), GeometryFactory().GenerateUnitCube().UniformScale(1.3f) );
	//cube->SetRotationalVelocity( GeoVector(0,1,0), 20 );
//	scene_objects.push_back( cube );
    
    
    focused_tile = 0;
    
    const float separation = 0.73f;
    const float slice_size = 2;
    const float slice_width = slice_size;
    const float slice_height = slice_size;
    const float tile_width = slice_width / 3.0f;
    const float tile_height = slice_height / 3.0f;
    const float tile_spacing = 0.03f;
    const float num_tiles_per_row = 3;
    const float num_tiles_per_col = 3;
    
    const float start_x = -slice_width / 2.0f - (num_tiles_per_row-1) / 2.0f;
    
    Geometry tile_geometry = GeometryFactory().GenerateXYUnitSquare().Scale(tile_width, tile_height, 1 );
    
    
    red_tex = Texture( 32, 32, Color::Red() );
    o_tex = Texture( 32, 32, Color::Green() );
    x_tex = Texture( 32, 32, Color::Blue() );
    
    
    tile_tex = Texture( "assets/metalgrate.jpg" );
    
    for(  int z = 0; z < 3; z++ )
    for( int x = 0; x < 3; x++ )
    for( int y = 0; y < 3; y++ )
    {
        RenderableObject* tile = new RenderableObject( tile_tex, tile_geometry);
        tile->Translate( -slice_width / 2.0f + tile_width * x + x * tile_spacing, -slice_height / 2.0f + tile_height * y + y * tile_spacing,  separation - z * separation );
        tile->SetUserData( "X", IntToString(x) );
        tile->SetUserData( "Y", IntToString(y) );
        tile->SetUserData( "Z", IntToString(z) );
        
        scene_objects.push_back(tile);
        tiles.push_back(tile);
    }
    
    
   // RenderableObject* slice = new RenderableObject( Texture( "assets/metalgrate.jpg" ), GeometryFactory().GenerateXYUnitSquare() + GeometryFactory().GenerateXYUnitSquare().Translate(0, 0, 0.4f) + GeometryFactory().GenerateXYUnitSquare().Translate( 0, 0, -0.4f ) );
    
   // slices.push_back(slice);
   // slice->Scale( 1.8f, 1.8f, 1.8f );
   // scene_objects.push_back(slice);
    
    
    cube->RotateInObjectspace( GeoVector( 1, 0, 0 ), -360 );
    
    tiles[0]->SetTexture(red_tex);
    
    last_mousedown_x = 0;
    last_mousedown_y = 0;
    
 //   cube->SetRotationalVelocity( GeoVector( 1, 0, 0 ), 100, 20 );
    
}

void SpinningCubeScene::HandleLeftMouseDown( const unsigned int x, const unsigned int y )
{
    printf( "got left mouse down with x,y of %i, %i\n", x, y );
    cube->SetRotationalVelocity(GeoVector(0,1,0), 0);
    last_mousedown_x = x;
    last_mousedown_y = y;
    movements.clear();
    
    PickTiles( x, y );
}

void SpinningCubeScene::HandleLeftMouseUp( const unsigned int x, const unsigned int y )
{
    printf( "got left mouse up with x,y of %i, %i\n", x, y ); 
    ComputeSpin(x, y, true);
     SnapCubeToAxis();
    movements.clear();
}

void SpinningCubeScene::HandleMouseMove( const unsigned int x, const unsigned int y )
{    
    ComputeSpin(x, y, false);
}

void SpinningCubeScene::PickTiles( const unsigned int x, const unsigned int y )
{
    std::vector<RenderableObject*> selected_tile_candidates;
    
    for( unsigned int i = 0; i < tiles.size(); i++ )
    {
        tiles[i]->SetTexture( tile_tex );
        
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
        focused_tile->SetTexture(red_tex);
        
        char focused_tile_info_buffer[256];
        sprintf( focused_tile_info_buffer, "(%s, %s, %s)\n", focused_tile->GetUserData("X").c_str(), focused_tile->GetUserData("Y").c_str(), focused_tile->GetUserData("Z").c_str() );
        printf( "focused tile info is %s\n", focused_tile_info_buffer );
        
    }
    
    
}

void SpinningCubeScene::ComputeSpin( const int x, const int y, const bool mouseup )
{
    const int delta = 1;
    if( abs(x - last_mousedown_x) < delta  && abs(y - last_mousedown_y) < delta )
        return;
    
    GeoVector swipe_direction = GeoVector( x - last_mousedown_x, y - last_mousedown_y );
    movements.push_back( swipe_direction );
    
    if( movements.size() < 4 )
        return;
    
    
    GeoVector overall_movement;
    for( unsigned int i = std::max<unsigned int>( 0, movements.size() - 12 ); i < movements.size(); i++ )
        overall_movement += movements[i];
    
    if( fabs( overall_movement.x ) < fabs( overall_movement.y ) )
        swipe_direction.x = 0;
    else 
        swipe_direction.y = 0;
    
    if( swipe_direction.x == 0 && swipe_direction.y == 0 )
        return;
    
    
    
    
    
    if( fabs( swipe_direction.x ) < fabs( swipe_direction.y ) )
        swipe_direction.x = 0;
    else 
        swipe_direction.y = 0; 
    
    if( !swipe_direction.x && !swipe_direction.y )
        return;
        
    

    
    printf( "swipe direction is %f, %f\n", swipe_direction.x, swipe_direction.y );
    
    GeoVector rotation_axis = swipe_direction.InvertXY().Normalize();
    
    printf( "after normalize roation axis is %f, %f, %f, %f\n", rotation_axis.x, rotation_axis.y, rotation_axis.z, rotation_axis.w );
    
    
    if( !mouseup )
    {
        cube->RotateInObjectspace( rotation_axis, swipe_direction.Length()*3 );
            
        for( unsigned int i = 0; i < tiles.size(); i++ )
            tiles[i]->RotateInWorldspace( rotation_axis, swipe_direction.Length()*3 );
        
    }
    else
    {
       
        
        
        //cube->SetRotationalVelocity( last_rotation_axis, swipe_direction.Length()*50, 40);
    }
    
    last_mousedown_x = x;
    last_mousedown_y = y;   
    last_rotation_axis = rotation_axis;
}

void SpinningCubeScene::SnapCubeToAxis()
{
    std::vector<GeoVector> standard_axes;
    standard_axes.push_back( GeoVector(1, 0, 0) );
    standard_axes.push_back( GeoVector(-1, 0, 0) );
    standard_axes.push_back( GeoVector(0, -1, 0) );
    standard_axes.push_back( GeoVector(0, -1, 0) );
    standard_axes.push_back( GeoVector(0, 0, -1) );
    standard_axes.push_back( GeoVector(0, 0, -1) );
    GeoVector closest_standard_axis = standard_axes[0];
    float largest_dot = fabs( standard_axes[0].Dot(cube->GetObjectspaceAxis() ) );
    
    for( unsigned int i = 1; i < standard_axes.size(); i++ )
    {
        if( fabs(standard_axes[i].Dot(cube->GetObjectspaceAxis())) > largest_dot )
        {
            largest_dot = fabs(standard_axes[i].Dot(cube->GetObjectspaceAxis()));
            closest_standard_axis = standard_axes[i];
            
            //cube->SetObjectspaceRotation( standard_axes[i], cube->GetObjectspaceAngle() );
            
        }
        
        
    }
    
      printf( "here cube axis angle is %f %s", cube->GetObjectspaceAngle(), cube->GetObjectspaceAxis().ToString().c_str()  );
    
    GeoVector axis = cube->GetObjectspaceAxis();
    if( fabs(axis.x) > 0.9f) 
        axis.x = copysign( 1.0f, axis.x );
    if( fabs( axis.x ) < 0.25f  )
        axis.x = 0;
    
    if( fabs(axis.y) > 0.9f) 
        axis.y = copysign( 1.0f, axis.y );
    if( fabs( axis.y ) < 0.25f  )
        axis.y = 0;
    
    if( fabs(axis.z) > 0.9f) 
        axis.z = copysign( 1.0f, axis.z );
    if( fabs( axis.z ) < 0.25f  )
        axis.z = 0; 
    
    
    
   // axis.z = 0;
   
    
    
    
   // axis.z = 0;
  //  axis = axis.Normalize();
    
    if( axis.x || axis.y || axis.z )
        cube->SetObjectspaceRotation(axis.Normalize(), cube->GetObjectspaceAngle() );
    
  
    
  //  cube->SetObjectspaceRotation( closest_standard_axis, cube->GetObjectspaceAngle() );
    
    printf( "after cube axis angle is %f %s", cube->GetObjectspaceAngle(), cube->GetObjectspaceAxis().ToString().c_str()  );
    
}

