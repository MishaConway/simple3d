#pragma once

#include "Object.h"
#include "../geo/Geometry.h"
#include <string>
#include <map>

class RenderableObject : public Object
{
public:
	RenderableObject();
	RenderableObject( Texture t, VertexBuffer v);
	RenderableObject( Texture t, Geometry geometry );
	virtual bool IsRenderable();
	virtual bool Render();
	virtual void SetVisible( const bool visible ); 
	bool IsVisible();
    
    virtual void SetTwoSided( const bool two_sided );
    virtual void SetColor( const Color color );
    
	virtual void SetPlanarReflector( const bool is_planar_reflector );
	virtual void SetBlendType( const BlendType::GRAPHICS_DEVICE_STATE_MANAGER_BLEND_TYPE blend_type );
	bool IsPlanarReflector();
	virtual void SetTechnique( const std::string& technique_name );
	static void EnableGlobalTechnique( const std::string& global_technique );
	static void DisableGlobalTechnique();
	virtual std::string GetDefaultTechnique();
	std::string GetTechnique();
	virtual VertexBuffer& GetVertexBuffer();
	Texture& GetTexture();
	Texture& GetGlowmap();
	void SetTexture( const Texture& tex );
	void SetGlowmap( const Texture& tex );
	std::vector<Vertex> GetWorldspaceVertices();
	virtual GeoVector GetWorldspaceCentroid();

	static void SetRootAssetsPath( const std::string& root_assets_path );
protected:
	void SetDefaultValues();
protected:
	Texture tex, glowmap;
	VertexBuffer vertex_buffer;
	bool visible;
	bool planar_reflector;
    bool two_sided;
    Color color;
	BlendType::GRAPHICS_DEVICE_STATE_MANAGER_BLEND_TYPE blend_type;
	std::string technique_name;
	static std::string enforced_global_technique;
	static std::string root_assets_path;
};