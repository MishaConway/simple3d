#include "BaseGraphicsDeviceStateManager.h"

BaseGraphicsDeviceStateManager::BaseGraphicsDeviceStateManager()
{
	locked = false;
	blend_state_locked = false;
}

void BaseGraphicsDeviceStateManager::Lock()
{
	if( locked )
		throw "graphics device state manager is already locked!";
	locked = true;
}

void BaseGraphicsDeviceStateManager::Unlock()
{
	locked = false;	
}

void BaseGraphicsDeviceStateManager::LockBlendState()
{
	if( blend_state_locked )
		throw "blend state already locked!";
	blend_state_locked = true;
}

void BaseGraphicsDeviceStateManager::UnlockBlendState()
{
	blend_state_locked = false;
}

void BaseGraphicsDeviceStateManager::SetDefaults()
{
	SetDefaultFrontFaceRendering();
   	SetDefaultBlendState();
}

void BaseGraphicsDeviceStateManager::SetBlendType( const BlendType::GRAPHICS_DEVICE_STATE_MANAGER_BLEND_TYPE blend_type )
{
	switch( blend_type )
	{
		case BlendType::NONE:
			SetDefaultBlendState();
			break;
		case BlendType::ALPHA:
			EnableAlphaBlending();
			break;
		case BlendType::ADDITIVE:
			EnableAdditiveBlending();
			break;
		default:
			throw "invalid blend type passed";
	}
}
