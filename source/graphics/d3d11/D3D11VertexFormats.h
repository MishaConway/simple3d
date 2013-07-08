#pragma once
#ifdef _WIN32

#include <d3d11_1.h>
//#include <xnamath.h>
#include <DirectXMath.h>
using namespace DirectX;

#define VERTEX_LAYOUT_NUM_ELEMENTS 5
extern D3D11_INPUT_ELEMENT_DESC d3d11_vertex_layout[];

#endif