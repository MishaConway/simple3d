#pragma once

#include "Camera.h"
#include "Sprite.h"
#include "Nebula.h"
#include <map>
#include "../graphics/window_type.h"
#include "../system/FileIO.h"

class Scene
{
public:
	Scene();
	Scene(  WINDOW_TYPE hWnd, const unsigned int width, const unsigned int height, const float fovy, const float near_z, const float far_z, const std::string& root_shader_path, const std::string& root_assets_path );
	
    void SetWidthAndHeight( const unsigned int width, const unsigned int height );
    
    void Draw();
	void SetBackgroundColor( const Color& background_color );
	virtual bool Update( const float elapsed_seconds );	
	virtual void HandleMouseMove( const unsigned int x, const unsigned int y ); 
	virtual void HandleLeftMouseDown( const unsigned int x, const unsigned int y ); 
    virtual void HandleLeftMouseUp( const unsigned int x, const unsigned int y );
	virtual void HandleRightMouseDown( const unsigned int x, const unsigned int y ); 
	virtual void HandleKeyDown( const char key );
	virtual void HandleKeyUp( const char key );

	static bool SetRootShaderPath( const std::string& root_shader_path );
    static std::string GetRootShaderPath();
	static bool SetRootAssetsPath( const std::string& root_assets_path );
	static std::string GetRootAssetsPath();
protected:
	virtual void PreRender();
	virtual bool Render();
	virtual void RenderScene( const bool reflection = false );
	virtual void RenderSprites();
    
    void SetupRenderTarget( RenderTarget render_target );

	void SetDefaults();
	void ConfigureCameraShaderValues( const bool update_projection = true );

	void RenderObjects( std::vector< RenderableObject* > objects );
protected:
	GraphicsDevice graphics_device;
	Camera camera;
    unsigned int width, height;
	
	//std::map< std::string, Effect> effects;
	std::vector< RenderableObject* > scene_objects;
	std::vector< RenderableObject* > sprites;

	RenderTarget render_target;
	RenderTarget downsample_render_target;
	RenderTarget downsample_render_target2;

	bool perform_prerendering;

	Color background_color;
private:
	static std::string root_shader_path, root_assets_path;
};
