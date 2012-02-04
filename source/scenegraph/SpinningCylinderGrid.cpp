#include "SpinningCylinderGrid.h"
#include "../geo/GeoFloat.h"
#include <math.h>
#include <algorithm>
#include "../std/nullptr.h"

#ifdef _WIN32
#define copysign _copysign
#endif

GridTile::GridTile( const std::string label, Texture t )
{
	this->label = label;
	this->texture = t;
}

std::string GridTile::GetLabel()
{
	return label;
}

Texture GridTile::GetTexture()
{
	return texture;
}

SpinningCylinderGrid::SpinningCylinderGrid(  const float radius, const float height, const unsigned int num_rows, const float tile_aspect_ratio )
{
	outline_blend_texture = Texture( "ui\\outerbox-blue.png");
	pSelectedTile = nullptr;

	angular_deceleration = 40.0f;
	angular_velocity = 0;
	spinning = false;
	this->radius = radius;
	this->height = height;
	this->num_rows = num_rows;
	this->tile_aspect_ratio = tile_aspect_ratio;
	degrees_scrolled = 0;

	row_spacing = height * 0.02f;
	row_height = height / (float) num_rows;
	circumference = GEO_2PI * radius;
	col_spacing = 0.06f;
	float width_per_tile = row_height * tile_aspect_ratio;
	float num_tiles_in_ring = circumference / width_per_tile - 1;
	const float extra_space = circumference - (int) num_tiles_in_ring * width_per_tile;
	width_per_tile += extra_space / (num_tiles_in_ring );
	num_tiles_in_ring = circumference / width_per_tile - 1;
	degrees_per_tile = 360.0f / (float) num_tiles_in_ring;
	num_tiles_per_circumference = (unsigned int) num_tiles_in_ring;

	render_front_faces = true;

	for( unsigned int i = 0; i < num_rows; i++ )
		grid_tiles.push_back( std::vector<GridTile>() );

	tile_frontside_texture = Texture( 256, 256 );
	tile_frontside_texture.ClearColor( Color::Blue() );
	quad = new Quad( tile_frontside_texture );

	tile_backside_texture = Texture( "ui\\kinectgrid.png");
}

void SpinningCylinderGrid::AddTile( const GridTile& grid_tile )
{
	unsigned int row_to_add_to = 0, smallest_row_size = grid_tiles.front().size();
	for( unsigned int i = 1; i < grid_tiles.size(); i++ )
		if( grid_tiles[i].size() < smallest_row_size )
		{
			row_to_add_to = i;
			smallest_row_size = grid_tiles[i].size();
		}
	grid_tiles[row_to_add_to].push_back( grid_tile );
}

void SpinningCylinderGrid::SetFrontFaceRendering()
{
	render_front_faces = true;
}

void SpinningCylinderGrid::SetBackFaceRendering()
{
	render_front_faces = false;
}

void SpinningCylinderGrid::Spin( const float impulse )
{
	spinning = true;
	angular_velocity = impulse;
}

bool SpinningCylinderGrid::Update( const float elapsed_seconds )
{
	if( spinning )
	{		
		const float old_angular_velocity = angular_velocity;
		angular_velocity -= (float)copysign( angular_deceleration * elapsed_seconds, angular_velocity ); 
		spinning = (int)copysign( 1, old_angular_velocity ) == (int)copysign( 1, angular_velocity );  
		degrees_scrolled += angular_velocity * elapsed_seconds;
	}
	return true;
}

bool SpinningCylinderGrid::IsSelected()
{
	return nullptr != pSelectedTile;
}

GridTile* SpinningCylinderGrid::GetSelectedTile()
{
	return pSelectedTile;
}

GeoVector SpinningCylinderGrid::GetWorldspacePositionOfSelectedTile()
{
	return worldspace_position_of_selected_tile;		
}

GeoVector SpinningCylinderGrid::GetWorldspaceNormalOfSelectedTile()
{
	return worldspace_normal_of_selected_tile;
}

float SpinningCylinderGrid::GetRadius()
{
	return radius;
}

void SpinningCylinderGrid::DeSelect()
{
	pSelectedTile = nullptr;
}

GridTile* SpinningCylinderGrid::PickTileFromScreenSpaceCoordinates( const unsigned int screenspace_left, const unsigned int screenspace_right, const unsigned int screenspace_top, const unsigned int screenspace_bottom, const unsigned int viewport_width, const unsigned int viewport_height, Camera camera )
{	
	pSelectedTile = nullptr;

	std::vector<TransformedGridTile> transformed_grid_tiles = GetTransformedGridTiles();
	for( unsigned int i = 0; i < transformed_grid_tiles.size(); i++ )
	{ 
			float max_x = 0, max_y = 0, min_x =0,  min_y = 0;
			for( unsigned int v = 0; v < transformed_grid_tiles[i].worldspace_vertices.size(); v++ )
			{ 
				transformed_grid_tiles[i].worldspace_vertices[v].position = camera.ProjectIntoScreenspace( GeoVector( transformed_grid_tiles[i].worldspace_vertices[v].position ) ).ToGeoFloat3(); 
				if( 0 == v )
				{
					max_x = min_x = transformed_grid_tiles[i].worldspace_vertices[v].position.x;
					max_y = min_y = transformed_grid_tiles[i].worldspace_vertices[v].position.y;
				}
				else
				{
					max_x = std::max( max_x,  transformed_grid_tiles[i].worldspace_vertices[v].position.x );
					min_x = std::min( min_x,  transformed_grid_tiles[i].worldspace_vertices[v].position.x );
					max_y = std::max( max_y,  transformed_grid_tiles[i].worldspace_vertices[v].position.y );
					min_y = std::min( min_y,  transformed_grid_tiles[i].worldspace_vertices[v].position.y );
				}
			}

			if( !(screenspace_left > max_x || min_x > screenspace_right  || screenspace_top > max_y || min_y > screenspace_bottom ) )
			{
				GeoVector worldspace_position = transformed_grid_tiles[i].worldspace_centroid;
				GeoVector worldspace_normal = (worldspace_position - GetWorldspaceCentroid()).Normalize();
				const float dot = worldspace_normal.Dot( -camera.GetEyeDirectionNormalized() );
				const GeoVector eye_dir = -camera.GetEyeDirectionNormalized();
				//need to make sure tile is front facing with respect to the camera
				if( worldspace_normal.Dot( -camera.GetEyeDirectionNormalized() ) > 0.7f )
				{
					pSelectedTile = transformed_grid_tiles[i].pGridTile;
					worldspace_position_of_selected_tile = worldspace_position;
					worldspace_normal_of_selected_tile = worldspace_normal;
					return pSelectedTile;
				}				
			}	
		}
	return pSelectedTile;
}

bool SpinningCylinderGrid::Render()
{
	if( render_front_faces )
		pGraphicsDevice->GetStateManager().SetDefaultFrontFaceRendering();
	else
		pGraphicsDevice->GetStateManager().SetDefaultBackFaceRendering();
	pGraphicsDevice->GetStateManager().Lock();
	
	unsigned int row_of_selected_tile;
	float angle_of_selected_tile;

	std::vector<TransformedGridTile> transformed_grid_tiles = GetTransformedGridTiles();
	for( unsigned int i = 0; i < transformed_grid_tiles.size(); i++ )
	{
		TransformQuad( transformed_grid_tiles[i].row, transformed_grid_tiles[i].angle, 0, 1 );
		
		if( render_front_faces  )
		{
			if( pSelectedTile == transformed_grid_tiles[i].pGridTile )
			{
				row_of_selected_tile = transformed_grid_tiles[i].row;
				angle_of_selected_tile = transformed_grid_tiles[i].angle;
			//	quad->SetTexture( tile_backside_texture );
			}
			//else
				quad->SetTexture( transformed_grid_tiles[i].pGridTile->GetTexture() );
		}
		else
		{
			if( pSelectedTile == transformed_grid_tiles[i].pGridTile )
				quad->SetTexture( transformed_grid_tiles[i].pGridTile->GetTexture() );
			else
				quad->SetTexture( transformed_grid_tiles[i].pGridTile->GetTexture() );

		}
		//if( pSelectedTile != pGridTile )
			quad->Render();	
	}

	pGraphicsDevice->GetStateManager().Unlock();

	if( render_front_faces && pSelectedTile )
	{
		TransformQuad( row_of_selected_tile, angle_of_selected_tile, 0.001f, 1.05f );
		//pGraphicsDevice->SetBlendState( transparency_blend_state );
		pGraphicsDevice->GetStateManager().EnableAlphaBlending();
		pGraphicsDevice->GetStateManager().LockBlendState();
		quad->SetTexture( outline_blend_texture );
		quad->Render();
		pGraphicsDevice->GetStateManager().UnlockBlendState();
		pGraphicsDevice->GetStateManager().DisableBlending();
		//pGraphicsDevice->SetDefaultBlendState();	
	}
	return true;	
}

std::vector<TransformedGridTile> SpinningCylinderGrid::GetTransformedGridTiles()
{
	std::vector<TransformedGridTile> transformed_grid_tiles;
	for( unsigned int row = 0; row < grid_tiles.size(); row++ )
	for( unsigned int j = 0; j < num_tiles_per_circumference && j < grid_tiles[row].size(); j++ )
	{			
		const float angle = (float) j * degrees_per_tile + degrees_scrolled;
		TransformQuad( row, angle, 0, 1 );
		bool stop = false;
		TransformedGridTile t;
		t.angle = angle;
		t.row = row;
		t.pGridTile = &grid_tiles[row][j];
		t.worldspace_vertices = quad->GetWorldspaceVertices();
		t.worldspace_centroid = quad->GetWorldspaceCentroid();
		transformed_grid_tiles.push_back( t );	
	}
	return transformed_grid_tiles;
}

void SpinningCylinderGrid::TransformQuad( const unsigned int row, const float angle, const float radius_offset, const float scale_factor )
{
	quad->SetIdentity();
	quad->Scale( row_height * tile_aspect_ratio*scale_factor, row_height*scale_factor, 1.0f );		
	quad->Translate( 0, -0.13f + height / 2.0f -(row_height + row_spacing ) * (float) row, radius + radius_offset );
	quad->RotateY( angle  );
}

GeoVector SpinningCylinderGrid::GetWorldspaceCentroid()
{
	return GeoVector(0, 0, 0);
}