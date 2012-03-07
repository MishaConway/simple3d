#pragma once

#include "include_opengl.h"
#include "../BaseGraphicsDeviceStateManager.h"

class OpenGLGraphicsDeviceStateManager : public BaseGraphicsDeviceStateManager
{
public:
	OpenGLGraphicsDeviceStateManager();
	
	virtual void SetDefaultFrontFaceRendering();
	virtual void SetDefaultBackFaceRendering();
    virtual void SetDefaultFrontAndBackRendering();
    
	virtual void SetParticleRendering();
	virtual void SetSpriteRendering( const BlendType::GRAPHICS_DEVICE_STATE_MANAGER_BLEND_TYPE blend_type = BlendType::NONE );
	
	virtual void EnableAlphaBlending();
	virtual void EnableAdditiveBlending();
	virtual void DisableBlending();
protected:
	virtual void SetDefaultBlendState();

private:
};