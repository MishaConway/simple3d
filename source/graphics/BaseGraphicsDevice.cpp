#include "BaseGraphicsDevice.h"

void BaseGraphicsDevice::SetViewport( const unsigned int width, const unsigned int height )
{
    viewport.Width = (float)width;
    viewport.Height = (float)height;
    viewport.MinDepth = 0;
    viewport.MaxDepth = 1;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
}

GraphicsDeviceViewport BaseGraphicsDevice::GetViewport()
{
	return viewport;
}
