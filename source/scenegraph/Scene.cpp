#include "Scene.h"
#include "../string/string_utils.h"
#include <algorithm>

std::string Scene::root_shader_path;
std::string Scene::root_assets_path;


Scene::Scene(){}
Scene::Scene(  WINDOW_TYPE hWnd, const unsigned int width, const unsigned int height, const float fovy, const float near_z, const float far_z, const std::string& root_shader_path, const std::string& root_assets_path )
{		
    this->width = width;
    this->height = height;
    
    SetRootShaderPath(root_shader_path);
    SetRootAssetsPath(root_assets_path);

    graphics_device.Initialize( hWnd, width, height, true );
	Object::BindGraphicsDevice( &graphics_device );
        
	SetBackgroundColor( Color::Black() );

    render_target = RenderTarget( 1024, 1024 );
    


	downsample_render_target = RenderTarget( 256, 256 );
	downsample_render_target2 = RenderTarget( 256, 256 );
        
    std::string shader_path = graphics_device.GetRendererType() == "D3D11" ? GetRootShaderPath() + "/hlsl/shaders.fx" : GetRootShaderPath() + "glsl/techniques.fx";
    
    
    if( !File::Exists( shader_path ))
    {
        printf( "ERROR!!! cannot find shader at %s\n", shader_path.c_str() );
      //  throw "could not load shader!";
    }
 
	Effect( shader_path.c_str() );
    
	camera = Camera( width, height, fovy, near_z, far_z, GeoVector(0, 7, 14 ), GeoVector( 0, 0, 0 ) );

	perform_prerendering = false;
}

void Scene::SetWidthAndHeight( const unsigned int width, const unsigned int height )
{
    this->width = width;
    this->height = height;
    graphics_device.SetViewport(width, height);
    camera.SetWidthHeight(width, height);
}

bool Scene::SetRootShaderPath( const std::string& _root_shader_path )
{
	root_shader_path = _root_shader_path;
    if( !EndsWith( root_shader_path, "/") )
        root_shader_path += "/";
	return Directory::Exists( root_shader_path  );
}

std::string Scene::GetRootShaderPath()
{
    return root_shader_path;
}

bool Scene::SetRootAssetsPath( const std::string& _root_assets_path )
{
	root_assets_path = _root_assets_path;
    if( !EndsWith( root_assets_path, "/") )
        root_assets_path += "/";
    
	RenderableObject::SetRootAssetsPath( root_assets_path );
	return Directory::Exists( root_assets_path );
}

std::string Scene::GetRootAssetsPath()
{
	return root_assets_path;	
}

void Scene::SetBackgroundColor( const Color& background_color )
{
	this->background_color = background_color;
}

void Scene::RenderScene( const bool reflection )
{	
	if( !reflection )
		graphics_device.GetStateManager().SetDefaultFrontFaceRendering();
	else
		graphics_device.GetStateManager().SetDefaultBackFaceRendering();
	graphics_device.GetStateManager().Lock();
    
    
    RenderObjects( scene_objects );
    
    graphics_device.GetStateManager().Unlock();
}

void Scene::RenderSprites()
{
	RenderObjects( sprites );
}

void Scene::ConfigureCameraShaderValues( const bool update_projection )
{	
	graphics_device.SetViewport( camera.GetWidth(), camera.GetHeight() );
    Effect::GetCurrentEffect().SetMatrix( "ViewTransform", camera.GetViewTransform() );
    if( update_projection )
	  Effect::GetCurrentEffect().SetMatrix( "ProjectionTransform", camera.GetProjectionTransform() );
	Effect::GetCurrentEffect().SetFloat( "viewport_width", graphics_device.GetViewport().Width );
	Effect::GetCurrentEffect().SetFloat( "viewport_height", graphics_device.GetViewport().Height );	
	Effect::GetCurrentEffect().SetFloatArray( "eye_position", camera.GetEyePosition() );	
	
}

void Scene::SetupRenderTarget( RenderTarget _render_target )
{
    graphics_device.SetRenderTarget( _render_target, Color::Black() );
    graphics_device.SetViewport( _render_target.GetTexture().GetWidth(), _render_target.GetTexture().GetHeight() );
    camera.SetWidthHeight( _render_target.GetTexture().GetWidth(), _render_target.GetTexture().GetHeight() );
}

void Scene::PreRender()
{
	if( !perform_prerendering )
		return;
	
	graphics_device.GetStateManager().SetDefaults();
	ConfigureCameraShaderValues();

	
    //SetupRenderTarget( downsample_render_target );
    //RenderableObject::EnableGlobalTechnique( "GlowFill" );
    //RenderScene();
    //RenderableObject::DisableGlobalTechnique();
		
    graphics_device.SetDefaultRenderTarget();
    //downsample_render_target.GetTexture().SaveToFile( "/Users/misha/Documents/beforedownsample.png", true ); 
				
    //downsample_render_target.HorizontalBlur( downsample_render_target2 );
    //downsample_render_target2.VerticalBlur( downsample_render_target );
    //downsample_render_target2.GetTexture().SaveToFile( "/Users/misha/Documents/downsampledafterhorizontal.png", true );
	

	// REFLECTION FILL PASSES
	for( unsigned int i = 0; i < scene_objects.size(); i++ )
	if( scene_objects[i]->IsPlanarReflector() )
	{
		const bool object_visible = scene_objects[i]->IsVisible();
		scene_objects[i]->SetVisible( false );
		Effect::GetCurrentEffect().SetInt( "clipping_enabled", 1 );
		
        
        SetupRenderTarget( render_target );
		GeoVector reflection_plane( 0, 1, 0, scene_objects[i]->GetWorldspaceCentroid().y);
		//reflection_plane.w = 0;
		ConfigureCameraShaderValues( false ); //this is the thing that is fucking up reflections
      
        
        
        
        
		Effect::GetCurrentEffect().SetFloatArray( "clip_plane",  reflection_plane );
		Effect::GetCurrentEffect().SetMatrix( "ViewTransform", camera.GetReflectedViewTransform( reflection_plane ) );
		Effect::GetCurrentEffect().SetTexture( "reflection", render_target.GetTexture() );

		graphics_device.Clear( Color::Black() );
		
		RenderScene( true );
		Effect::GetCurrentEffect().SetInt( "clipping_enabled", 0 );
		scene_objects[i]->SetVisible( object_visible );

		graphics_device.SetDefaultRenderTarget();
		render_target.GetTexture().SaveToFile( "/Users/misha.conway/Documents/whoa.bmp", true );
	}


}

void Scene::SetDefaults()
{
	camera.SetWidthHeight( width, height );
    
    graphics_device.GetStateManager().SetDefaults();
	graphics_device.SetDefaultRenderTarget();
	graphics_device.SetViewport( width, height );
	graphics_device.Clear( background_color );

	//render_target.GetTexture().SaveToFile( "lalala.png", true );	
	ConfigureCameraShaderValues();
}

bool Scene::Render()
{
	Effect::GetCurrentEffect().SetFloatArray( "light_source",  GeoVector( -2, 15, 20, 1 ) );
    
    PreRender();
	SetDefaults();
	RenderScene();
	if( perform_prerendering )
		Sprite( downsample_render_target.GetTexture(), GeoFloat2( 0, 0), GeoFloat2( 2, 2 ), BlendType::ADDITIVE ).Render();
	RenderSprites();
	return true;
}

void Scene::Draw()
{
    Render();
	graphics_device.SwapBackBuffer();
}

void Scene::RenderObjects( std::vector< RenderableObject* > objects )
{		
	for( unsigned int i = 0; i < objects.size(); i++ )
		objects[i]->Render(); 
}

bool Scene::Update( const float elapsed_seconds )
{
	for( unsigned int i = 0; i < sprites.size(); i++ )
		sprites[i]->Update( elapsed_seconds );
		
	for( unsigned int i = 0; i < scene_objects.size(); i++ )
		scene_objects[i]->Update( elapsed_seconds );	
	
	
	return true;
}

void Scene::HandleMouseMove( const unsigned int x, const unsigned int y )
{
}

void Scene::HandleLeftMouseDown( const unsigned int x, const unsigned int y )
{
}

void Scene::HandleLeftMouseUp( const unsigned int x, const unsigned int y )
{    
}

void Scene::HandleRightMouseDown( const unsigned int x, const unsigned int y )
{
}

void Scene::HandleKeyDown( const char key )
{
	switch( key )
	{
		case 'R':
			camera.Translate( GeoVector(0, 0.2f, 0 ) );
			break;
		case 'F':
			camera.Translate( GeoVector(0, -0.2f, 0 ) );
			break;
	}
}

void Scene::HandleKeyUp( const char key )
{

}





