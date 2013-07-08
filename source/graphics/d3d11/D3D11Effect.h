#ifdef _WIN32
#pragma once

#define NOMINMAX
#include <Windows.h>
#include <d3d11_1.h>
//#include <d3dx11.h>
#include "includes/d3dx11effect.h"
#include <D3DCompiler.h>
#include "D3D11VertexBuffer.h"
#include "D3D11VertexFormats.h"
#include "D3D11ShaderResource.h"
#include "D3D11Texture.h"
#include "../BaseEffect.h"
#include "../../geo/GeoVector.h"
#include "../../geo/GeoMatrix.h"
//#include <xnamath.h>
#include <DirectXMath.h>
using namespace DirectX;
#include <string>
#include <vector>
#include <functional>


class D3D11Effect : public BaseEffect
{
friend class D3D11GraphicsDevice;
public:
 D3D11Effect();
 D3D11Effect( const std::string& effect_path );
 bool RenderTechnique( const std::string& technique_name, std::function<void()> f );

 void Enable();
 static D3D11Effect GetCurrentEffect();

 bool SetInt( const std::string& variable_name, const int i );
 bool SetFloat( const std::string& variable_name, float flt );
 bool SetFloatArray( const std::string& variable_name, XMFLOAT3& float_array );
 bool SetFloatArray( const std::string& variable_name, XMFLOAT4& float_array );
 virtual bool SetFloatArray( const std::string& variable_name, GeoVector float_array );
 bool SetFloatArray( const std::string& variable_name, std::vector<float>& float_array );
 bool SetMatrix( const std::string& variable_name, XMMATRIX& matrix );
 bool SetMatrix( const std::string& variable_name, XMFLOAT4X4& matrix );
 bool SetMatrix( const std::string& variable_name, GeoMatrix& matrix );
// bool SetShaderResource( const std::string& variable_name, ShaderResource& shader_resource );
 bool SetTexture( const std::string& variable_name, D3D11Texture& texture );
private:
 bool SetEffectVariable( const std::string& variable_name, std::function<void(ID3DX11EffectVariable*)> f );

private:
 ID3D11InputLayout* pVertexLayout;
 ID3DX11Effect* pEffect;
 ID3D11DeviceContext* pDeviceContext;
 static D3D11Effect current_effect;
 static D3D11GraphicsDevice* pGraphicsDevice;
};
#endif