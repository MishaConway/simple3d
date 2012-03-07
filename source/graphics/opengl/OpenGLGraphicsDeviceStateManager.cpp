#include "OpenGLGraphicsDeviceStateManager.h"

OpenGLGraphicsDeviceStateManager::OpenGLGraphicsDeviceStateManager()
{
}
	
void OpenGLGraphicsDeviceStateManager::SetDefaultFrontFaceRendering()
{
	if( locked )
		return;
	glFrontFace( GL_CW );
	glEnable( GL_CULL_FACE );
	glEnable(GL_DEPTH_TEST);
	glDepthMask( GL_TRUE );
}

void OpenGLGraphicsDeviceStateManager::SetDefaultBackFaceRendering()
{
	if( locked )
		return;
	glFrontFace( GL_CCW );
	glEnable( GL_CULL_FACE );
	glEnable(GL_DEPTH_TEST);
	glDepthMask( GL_TRUE );
}

void OpenGLGraphicsDeviceStateManager::SetDefaultFrontAndBackRendering()
{
	if( locked )
		return;    
    glDisable( GL_CULL_FACE );
	glEnable(GL_DEPTH_TEST);
	glDepthMask( GL_TRUE );
}

void OpenGLGraphicsDeviceStateManager::SetParticleRendering()
{
	if( locked )
		return;
	EnableAdditiveBlending();
	glDepthMask( GL_FALSE );
	glDisable( GL_CULL_FACE );
	glEnable(GL_DEPTH_TEST);
}

void OpenGLGraphicsDeviceStateManager::SetSpriteRendering( const BlendType::GRAPHICS_DEVICE_STATE_MANAGER_BLEND_TYPE blend_type )
{
	if( locked )
		return;
	glDisable( GL_CULL_FACE );
	glDisable( GL_DEPTH_TEST );
	SetBlendType( blend_type );
}

void OpenGLGraphicsDeviceStateManager::EnableAlphaBlending()
{
	if( locked || blend_state_locked )
		return;
	glEnable( GL_BLEND );
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLGraphicsDeviceStateManager::EnableAdditiveBlending()
{
	if( locked || blend_state_locked )
		return;
	glEnable( GL_BLEND );
	glBlendFunc(GL_ONE, GL_ONE);
}

void OpenGLGraphicsDeviceStateManager::DisableBlending()
{
	if( locked )
		return;
	SetDefaultBlendState();
}

void OpenGLGraphicsDeviceStateManager::SetDefaultBlendState()
{
	if( locked || blend_state_locked )
		return;
	glDisable( GL_BLEND );
}
