#pragma once
#include "Quad.h"
#include "Camera.h"

class GridTile
{
public:
	GridTile( const std::string label, Texture t );
	std::string GetLabel();
	Texture GetTexture();
private:
	std::string label;
	Texture texture;
};

struct TransformedGridTile
{
	GridTile* pGridTile;
	unsigned int row; 
	float angle;
	std::vector<Vertex>  worldspace_vertices;
	GeoVector worldspace_centroid;
};

class SpinningCylinderGrid : public RenderableObject
{
public:
	SpinningCylinderGrid( const float radius, const float height, const unsigned int num_rows, const float tile_aspect_ratio );
	virtual bool Render();
	virtual bool Update( const float elapsed_seconds );
	void Spin( const float impulse );
	void SetFrontFaceRendering();
	void SetBackFaceRendering();

	void AddTile( const GridTile& grid_tile );

	GridTile* PickTileFromScreenSpaceCoordinates( const unsigned int screenspace_left, const unsigned int screenspace_right, const unsigned int screenspace_top, const unsigned int screenspace_bottom, const unsigned int viewport_width, const unsigned int viewport_height, Camera camera );
	void DeSelect();

	bool IsSelected();
	GridTile* GetSelectedTile();
	GeoVector GetWorldspacePositionOfSelectedTile();
	GeoVector GetWorldspaceNormalOfSelectedTile();
	float GetRadius();
	virtual GeoVector GetWorldspaceCentroid();
private:
	std::vector<TransformedGridTile> GetTransformedGridTiles();
	void TransformQuad( const unsigned int row, const float angle, const float radius_offset, const float scale_factor );
private:
	bool render_front_faces;
	float radius, height, degrees_scrolled;
	float tile_aspect_ratio;
	unsigned int num_rows;
	Texture tile_frontside_texture, tile_backside_texture, outline_blend_texture;
	Quad* quad;



	std::vector<  std::vector<GridTile>  > grid_tiles;
	GridTile* pSelectedTile; 
	GeoVector worldspace_position_of_selected_tile;
	GeoVector worldspace_normal_of_selected_tile;
	
	float angular_deceleration, angular_velocity;
	bool spinning;

	float row_spacing, col_spacing, degrees_per_tile, row_height, circumference;
	unsigned int num_tiles_per_circumference;
};