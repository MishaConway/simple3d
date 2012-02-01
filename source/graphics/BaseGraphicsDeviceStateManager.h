#pragma once

namespace BlendType
{
	enum GRAPHICS_DEVICE_STATE_MANAGER_BLEND_TYPE
	{
		NONE, ALPHA, ADDITIVE
	};
};

class BaseGraphicsDeviceStateManager
{
public:
	BaseGraphicsDeviceStateManager();
	virtual void SetDefaults();
	
	virtual void SetDefaultFrontFaceRendering() = 0;
	virtual void SetDefaultBackFaceRendering() = 0;
	virtual void SetParticleRendering() = 0;
	virtual void SetSpriteRendering( const BlendType::GRAPHICS_DEVICE_STATE_MANAGER_BLEND_TYPE blend_type = BlendType::NONE ) = 0;
	
	virtual void SetBlendType( const BlendType::GRAPHICS_DEVICE_STATE_MANAGER_BLEND_TYPE blend_type );
	virtual void EnableAlphaBlending() = 0;
	virtual void EnableAdditiveBlending() = 0;
	virtual void DisableBlending() = 0;

	virtual void Lock();
	virtual void Unlock();

	virtual void LockBlendState();
	virtual void UnlockBlendState();
protected:
	virtual void SetDefaultBlendState() = 0;
protected:
	bool locked, blend_state_locked;
};