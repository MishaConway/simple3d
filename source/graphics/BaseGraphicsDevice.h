#pragma once
#include <string>
#include "Color.h"
#include "../interfaces/IInitializable.h"


struct GraphicsDeviceViewport
{
	float TopLeftX, TopLeftY, Width, Height, MinDepth, MaxDepth;
}; 	

class BaseGraphicsDevice : public IInitializable
{
public:
	virtual void SetViewport( const unsigned int width, const unsigned int height );
	virtual void Clear( Color color ) = 0;
	GraphicsDeviceViewport GetViewport();
	virtual std::string GetRendererType() = 0;
protected:
	GraphicsDeviceViewport viewport;
};