#pragma once

#define RENDERER_OPENGL 1
#define RENDERER_D3D11  2

//#define RENDERER RENDERER_OPENGL
#define RENDERER RENDERER_D3D11

#if RENDERER == RENDERER_D3D11
#include "d3d11/D3D11GraphicsDevice.h"
typedef D3D11GraphicsDevice GraphicsDevice;
typedef D3D11VertexBuffer VertexBuffer;
typedef D3D11DynamicVertexBuffer DynamicVertexBuffer;
typedef D3D11Effect Effect;
typedef D3D11RenderTarget RenderTarget;
typedef D3D11Texture Texture;
#endif

#if RENDERER == RENDERER_OPENGL
#include "opengl/OpenGLGraphicsDevice.h"
typedef OpenGLGraphicsDevice GraphicsDevice;
typedef OpenGLVertexBuffer VertexBuffer;
typedef OpenGLDynamicVertexBuffer DynamicVertexBuffer;
typedef OpenGLEffect Effect;
typedef OpenGLRenderTarget RenderTarget;
typedef OpenGLTexture Texture;
#endif