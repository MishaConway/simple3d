#include "../Scene.h"
#include "../SpinningCylinderGrid.h"

class SpinningCylinderGridScene : public Scene
{
public:
	SpinningCylinderGridScene();
	SpinningCylinderGridScene( WINDOW_TYPE hWnd, const unsigned int width, const unsigned int height, const float fovy, const float near_z, const float far_z, const std::string& root_shader_path, const std::string& root_assets_path );
	void SelectCurrentHoverTile();
	void SpinCylinderGrid( const float impulse );
	virtual bool Update( const float elapsed_seconds );	
	virtual void HandleMouseMove( const unsigned int x, const unsigned int y ); 
	virtual void HandleLeftMouseDown( const unsigned int x, const unsigned int y ); 
protected:
	virtual void RenderScene( const bool reflection = false );
private:
	void UpdateCursor( const unsigned int x, const unsigned int y ); 

protected:
	float default_height, default_focus_height, distance_from_spinning_cylinder_grid;

	Sprite* pCursor;
	Sprite* pCursorProgressCircle;
	float cursor_progress;

	Sprite* pRightArrowSprite;
	Sprite* pLeftArrowSprite;
	Sprite* pRightArrowSpriteHighlighted;
	Sprite* pLeftArrowSpriteHighlighted;

	SpinningCylinderGrid* spinning_cylinder_grid;
	GridTile* pSelectedZoomOutTile;
	GridTile* pCurrentHoverTile;

	RenderableObject* pCircle;
	RenderableObject* pCylinder;
};