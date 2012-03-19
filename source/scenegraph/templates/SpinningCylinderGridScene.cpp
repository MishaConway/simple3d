#include "SpinningCylinderGridScene.h"
#include <algorithm>
#include "../../std/nullptr.h"

SpinningCylinderGridScene::SpinningCylinderGridScene(){}
SpinningCylinderGridScene::SpinningCylinderGridScene( HWND hWnd, const unsigned int width, const unsigned int height, const float fovy, const float near_z, const float far_z ) : Scene( hWnd, width, height, fovy, near_z, far_z )
{
	perform_prerendering = true;
	
	float offset = -1.5f;
    default_height = 1.4f + offset;
	default_focus_height = 1.19f + offset;
	distance_from_spinning_cylinder_grid = 4.2f;
	camera = Camera( width, height, fovy, near_z, far_z, GeoVector(0, default_height, distance_from_spinning_cylinder_grid ), GeoVector( 0, default_focus_height, 0 ) );
	
	spinning_cylinder_grid = new SpinningCylinderGrid( 2.25f, 1.3f, 4, 1.6f );
	std::vector<std::string> video_files = Directory::GetFiles( GetRootAssetsPath() + std::string("video_thumbnails"), true );
    for( unsigned int i = 0; i < video_files.size(); i++ )
        printf( "got video file of : %s\n", video_files[i].c_str() );
    

	for( unsigned int i = 0; i < video_files.size(); i++ )
		if( "db" != video_files[i].substr(video_files[i].find_last_of(".")+1) )
			spinning_cylinder_grid->AddTile( GridTile( "cool", Texture( video_files[i] ) ) );
	
	pCircle = new RenderableObject( Texture(GetRootAssetsPath() + "ui/kinectgrid.png"), GeometryFactory().GenerateUnitXZCircle().Scale(2.2f, 1, 2.2f ) );
	//pCircle->SetTexture( Texture(GetRootAssetsPath() + "17730-1920x1200.jpg") );
	pCircle->SetTexture( Texture(GetRootAssetsPath()+ "flux-studios-lineage-hourglass-stainless-steel-tile.jpg") );
	scene_objects.push_back( pCircle );
	pCircle->Translate( 0, -1.2f, 0 );
	pCircle->SetPlanarReflector( true );
	pCircle->GetTexture().SetTiling( 4.5f );
	pCircle->SetRotationalVelocity( GeoFloat3( 0, 1, 0 ), 3 );

	pCircle->SetBlendType( BlendType::ALPHA );

	Texture glow_circle = Texture( 128, 128 );
	const GeoFloat2 glow_circle_center( (float)glow_circle.GetWidth() / 2.0f, (float)glow_circle.GetHeight() / 2.0f );
	const float glow_circle_radius = glow_circle_center.x * 0.7f;
	#ifdef _WIN32	
    glow_circle.SetData( [this, &glow_circle_center, &glow_circle_radius]
    #endif
    #if defined(__APPLE__) || defined(__APPLE_CC__)  
    glow_circle.SetData( ^                 
    #endif
        (const unsigned int x, const unsigned int y, float* pRed, float* pGreen, float* pBlue, float* pAlpha){ 
		if( (x-glow_circle_center.x)*(x-glow_circle_center.x) + (y-glow_circle_center.y)*(y-glow_circle_center.y) > glow_circle_radius * glow_circle_radius )
		{
			*pRed = *pGreen = *pBlue = *pAlpha =  0.0f;
			*pBlue = 0.6f;
			*pAlpha = 0.6f;    
		}
		else
		{
			*pRed = *pGreen = *pBlue = *pAlpha = 1;
			*pGreen = 0;
			*pAlpha = 1.0f;
		}
	});


//	glow_circle.SaveToFile( "glowcircle.dds" );
	Texture cylinder_top = Texture(GetRootAssetsPath()+ "17730-1920x1200.jpg");
	Texture cylinder_bottom = Texture(GetRootAssetsPath()+ "17730-1920x1200.jpg");

	pCylinder = new RenderableObject( glow_circle, GeometryFactory().GenerateUnitCylinder().Scale(2.12f, 0.1f, 2.12f ) );
	pCylinder->SetBlendType( BlendType::ALPHA );

/*	
    pCylinder = new RenderableObject( Texture(glow_circle, cylinder_top, cylinder_bottom), GeometryFactory().GenerateUnitCappedCylinder().Scale(2.12f, 0.1f, 2.12f ) );
	//pCylinder = new RenderableObject( glow_circle, GeometryFactory().GenerateUnitCappedCylinder().Scale(2.12f, 0.1f, 2.12f ) );
	*/
	pCylinder->Translate( 0, -0.81f, 0 );
	pCylinder->SetRotationalVelocity( GeoFloat3( 0, 1, 0 ), -4 );
//	pCylinder->SetTexture( glow_circle );
	//pCylinder->GetTexture().SetTiling( .1f );
	pCylinder->SetGlowmap( glow_circle );
	scene_objects.push_back( pCylinder );

	const float arrow_margin = 0.001f;
	const GeoFloat2 arrow_size( 0.188235294f, 0.188235294f );
	pRightArrowSprite = new Sprite( Texture( GetRootAssetsPath() + "ui/arrow_right.png"), GeoFloat2( 1 - arrow_size.x - arrow_margin, -.4f), arrow_size );
	pLeftArrowSprite = new Sprite( Texture( GetRootAssetsPath() + "ui/arrow_left.png"), GeoFloat2( -1 + arrow_margin, -.4f), arrow_size );
	pRightArrowSpriteHighlighted = new Sprite( GetRootAssetsPath() + "ui/arrow_right_highlighted.png", GeoFloat2( 1 - arrow_size.x - arrow_margin, -.4f), arrow_size );
	pLeftArrowSpriteHighlighted = new Sprite( Texture( GetRootAssetsPath()+ "ui/arrow_left_highlighted.png"), GeoFloat2( -1 + arrow_margin, -.4f), arrow_size );
	pRightArrowSpriteHighlighted->SetVisible( false );
	pLeftArrowSpriteHighlighted->SetVisible( false );	
	sprites.push_back( pRightArrowSprite );
	sprites.push_back( pLeftArrowSprite );
	sprites.push_back( pRightArrowSpriteHighlighted );
	sprites.push_back( pLeftArrowSpriteHighlighted );
	
	pCursor = new Sprite( Texture( GetRootAssetsPath() + "ui/hand.png"), GeoFloat2( -.7f, 0.0f), GeoFloat2(0.188235294f, 0.188235294f) );
	pCursorProgressCircle = new Sprite( Texture( GetRootAssetsPath() + "ui/circle.png"), GeoFloat2( -.7f, 0.0f), GeoFloat2(0.258235294f, 0.258235294f) );
	pCursorProgressCircle->SetTechnique( "ProgressSprite" );
	cursor_progress = 0;
	sprites.push_back( pCursor );
	sprites.push_back( pCursorProgressCircle );



	pCurrentHoverTile = nullptr;
}

void SpinningCylinderGridScene::RenderScene( const bool reflection )
{
	Scene::RenderScene( reflection );
	spinning_cylinder_grid->SetBackFaceRendering();
	spinning_cylinder_grid->Render(); 

	spinning_cylinder_grid->SetFrontFaceRendering();
	spinning_cylinder_grid->Render(); 
}

void SpinningCylinderGridScene::SpinCylinderGrid( const float impulse )
{
	spinning_cylinder_grid->Spin( impulse );
}

void SpinningCylinderGridScene::SelectCurrentHoverTile()
{	
}

void SpinningCylinderGridScene::UpdateCursor( const unsigned int x, const unsigned int y )
{			
	float cursor_screenspace_width = (pCursor->GetSize().x / 2 ) * graphics_device.GetViewport().Width;
	float cursor_screenspace_height = (pCursor->GetSize().y / 2 ) * graphics_device.GetViewport().Height;	
	const float clipspace_x = ((float) x / graphics_device.GetViewport().Width) * 2 - 1;
	const float clipspace_y = (((float) graphics_device.GetViewport().Height - y) / graphics_device.GetViewport().Height) * 2 - 1;
	
	pRightArrowSpriteHighlighted->SetVisible( clipspace_x >= pRightArrowSprite->GetPosition().x  );
	pRightArrowSprite->SetVisible( clipspace_x < pRightArrowSprite->GetPosition().x   );
	pLeftArrowSpriteHighlighted->SetVisible( clipspace_x <= pLeftArrowSprite->GetPosition().x + pLeftArrowSprite->GetSize().x );
	pLeftArrowSprite->SetVisible( clipspace_x > pLeftArrowSprite->GetPosition().x + pLeftArrowSprite->GetSize().x  );


	if( pLeftArrowSpriteHighlighted->IsVisible() || pRightArrowSpriteHighlighted->IsVisible() )
	{
		pCurrentHoverTile = nullptr;
		cursor_progress = -0.6f;
		spinning_cylinder_grid->DeSelect();
	}
	else
	{
		GridTile* pHoveringGridTile = spinning_cylinder_grid->PickTileFromScreenSpaceCoordinates( x - (unsigned int)(cursor_screenspace_width/2), x + (unsigned int)(cursor_screenspace_width/2), y - (unsigned int)(cursor_screenspace_height/2), y + (unsigned int)(cursor_screenspace_height / 2), (unsigned int)graphics_device.GetViewport().Width, (unsigned int)graphics_device.GetViewport().Height, camera );
		if( pCurrentHoverTile != pHoveringGridTile )
		{
			pCurrentHoverTile = pHoveringGridTile;
			cursor_progress = -0.6f;
		}
	}

	pCursor->SetPosition( GeoFloat2( clipspace_x, clipspace_y ) );	
}

bool SpinningCylinderGridScene::Update( const float elapsed_seconds )
{
	const bool progress_circle_visible = pCurrentHoverTile && !camera.IsMoving();
	pCursorProgressCircle->SetVisible( progress_circle_visible && cursor_progress > 0 );
	if( progress_circle_visible )
	{
		pCursorProgressCircle->SetPosition( GeoFloat2(pCursor->GetPosition().x - .02f, pCursor->GetPosition().y - .03f ) );
		Effect::GetCurrentEffect().SetFloat( "progress", cursor_progress );
		
		cursor_progress += elapsed_seconds / 1.5f;
		if( cursor_progress >= 1 )
		{
			cursor_progress = -0.6f;
			SelectCurrentHoverTile();
		}
	}
	
	if( !camera.IsMoving() )
	{
		if( pRightArrowSpriteHighlighted->IsVisible() )
			spinning_cylinder_grid->Spin( -200 * elapsed_seconds );
		else if( pLeftArrowSpriteHighlighted->IsVisible() )
			spinning_cylinder_grid->Spin(  200 * elapsed_seconds );
	}
	
	camera.Update( elapsed_seconds );

	Scene::Update( elapsed_seconds );
	spinning_cylinder_grid->Update( elapsed_seconds );
		
	return true;
}

void SpinningCylinderGridScene::HandleMouseMove( const unsigned int x, const unsigned int y )
{
	UpdateCursor( x, y );
}

void SpinningCylinderGridScene::HandleLeftMouseDown( const unsigned int x, const unsigned int y )
{
	//UpdateCursor( x, y );
    SelectCurrentHoverTile();
}

