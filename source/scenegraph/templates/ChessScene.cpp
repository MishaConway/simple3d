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
	SetBackgroundColor( Color::Green() );
	RenderableObject* cube = new RenderableObject( Texture(GetRootAssetsPath() + "metalgrate.jpg" ), GeometryFactory().GenerateUnitCube().UniformScale(2) );
    
    GeoVector centroid = cube->GetWorldspaceCentroid();
    printf( "centroid is %f, %f, %f\n", centroid.x, centroid.y, centroid.z );
    
	cube->SetRotationalVelocity( GeoVector(0,1,0), 20 );
	//scene_objects.push_back( cube );
    
    
    std::string x = "2.7565136e-02";
    
    std::string y = "7.89";
    
    
    float f_y = StringToNumber<float>(y);
    
    printf( "f_y is %f\n", f_y);
    
    float f_x = StringToNumber<float>(x);
     printf( "f_x is %f\n", f_x);
    
   // std::string pawn_data = File::ReadAllText( GetRootAssetsPath() + "correct_pawn.simple" );
    std::string pawn_data = File::ReadAllText( GetRootAssetsPath() + "correct_pawn_subdivided_once.simple" );
    
    
    
   // printf( "pawn data is %s\n", pawn_data.c_str() );
    
   
   
    
    std::vector< std::string > pawn_data_lines = ExplodeString( pawn_data, "\n\r" );
    std::vector< GeoVertex > pawn_vertices;
    for( int i = 0; i < pawn_data_lines.size(); i++ )
    {
        const std::string line = StripString( pawn_data_lines[i], " \n\t\r" );
        if( line.size() > 0 && '#' != line[0] )
        {
        GeoVertex pawn_vertex;
        
      //  printf( "line %i is: %s\n", i, line.c_str() );
        
        std::vector< std::string > components = ExplodeString( line, "," );
        for( int j = 0; j < components.size(); j++ )
        {
           // printf( "\tcomponent:%s\n", components[j].c_str() );
            
            if( "p" == components[j] || "n" == components[j] )
            {
                GeoFloat3 float3;
                float3.x = StringToNumber<float>( components[j+1] );
                float3.y = StringToNumber<float>( components[j+2] );
                float3.z = StringToNumber<float>( components[j+3] );
                
                
                if( "p" == components[j] )
                    pawn_vertex.vertex.position = float3;
                else if( "n" == components[j] )
                    pawn_vertex.vertex.normal = float3;
                    
            }
            
            
            
        }
        
        
        pawn_vertices.push_back(pawn_vertex);
        }
        
    }
    
    Geometry pawn_geometry;
    for( int tri = 0; tri < pawn_vertices.size() / 3; tri++ )
    {
        GeoTriangle triangle( pawn_vertices[3*tri], pawn_vertices[3*tri+1], pawn_vertices[3*tri+2]);
        triangle.ReverseWinding();
        pawn_geometry.triangles.push_back(triangle);
        
        
        
    }
    
    RenderableObject* pawn = new RenderableObject( Texture(GetRootAssetsPath() + "metalgrate.jpg" ), pawn_geometry);
	scene_objects.push_back( pawn );
    
    pawn->SetRotationalVelocity( GeoVector(0,1,0), 20 );
    
    
    printf( "pawn vertices size is %i\n", pawn_vertices.size() );
    
    
    printf("ok\n");
    
    
}
