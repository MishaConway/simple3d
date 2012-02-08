#include "RenderableObject.h"

std::string RenderableObject::enforced_global_technique;
std::string RenderableObject::root_assets_path;

void RenderableObject::SetDefaultValues()
{
	visible = true;
	planar_reflector = false;
	blend_type = BlendType::NONE;
}

RenderableObject::RenderableObject()
{
	SetDefaultValues();
}

RenderableObject::RenderableObject( Texture t, VertexBuffer v) : tex(t), vertex_buffer(v)
{
	SetDefaultValues();
}

RenderableObject::RenderableObject( Texture t, Geometry geometry ) : tex(t)
{
	visible = true;
	planar_reflector = false;	
	vertex_buffer = VertexBuffer( geometry.GetVertices() );
}

void RenderableObject::SetRootAssetsPath( const std::string& _root_assets_path )
{
	root_assets_path = _root_assets_path;	
}

bool RenderableObject::IsRenderable()
{
	return true;
}

void RenderableObject::SetVisible( const bool visible )
{
	this->visible = visible;
}

bool RenderableObject::IsVisible()
{
	return visible;
}

void RenderableObject::SetBlendType( const BlendType::GRAPHICS_DEVICE_STATE_MANAGER_BLEND_TYPE blend_type )
{
	this->blend_type = blend_type;
}

void RenderableObject::SetPlanarReflector( const bool is_planar_reflector )
{
	planar_reflector = is_planar_reflector;
}

bool RenderableObject::IsPlanarReflector()
{
	return planar_reflector;
}

void RenderableObject::SetTechnique( const std::string& technique_name )
{
	this->technique_name = technique_name;
}

void RenderableObject::EnableGlobalTechnique( const std::string& global_technique )
{
	enforced_global_technique = global_technique;
}
	
void RenderableObject::DisableGlobalTechnique()
{
	enforced_global_technique.clear();
}

std::string RenderableObject::GetTechnique()
{
	return enforced_global_technique.empty() ? (technique_name.empty() ? GetDefaultTechnique() : technique_name) : enforced_global_technique;
}

std::string RenderableObject::GetDefaultTechnique()
{
	return "Basic";
}

bool RenderableObject::Render()
{
	if( visible )
	{
		Effect e = Effect::GetCurrentEffect();
		e.SetTexture( "lala", GetTexture() );
		e.SetTexture( "glowmap", GetGlowmap() );
		e.SetMatrix( "WorldTransform", GetWorldTransform() );
		e.SetMatrix( "WorldInverseTranspose", GetWorldInverseTranspose() );
		e.SetInt( "compute_projective_texture_coordinates", planar_reflector );
		pGraphicsDevice->SetVertexBuffer( GetVertexBuffer() );
		pGraphicsDevice->GetStateManager().SetDefaults();
			
		if( enforced_global_technique.empty() && planar_reflector )
		{
            BEGIN_RENDER_TECHNIQUE(e, "PlanarReflection")
                pGraphicsDevice->Draw( GetVertexBuffer().GetNumVertices() ); 
            END_RENDER_TECHNIQUE
		}
		else
		{		
			BEGIN_RENDER_CURRENT_TECHNIQUE(e)
                pGraphicsDevice->Draw( GetVertexBuffer().GetNumVertices() );
            END_RENDER_TECHNIQUE
		}
	}
	return true;
}

VertexBuffer& RenderableObject::GetVertexBuffer()
{
	return vertex_buffer;
}

Texture& RenderableObject::GetTexture()
{
	return tex;
}

Texture& RenderableObject::GetGlowmap()
{
	return glowmap;
}

void RenderableObject::SetTexture( const Texture& tex )
{
	this->tex = tex;
}

void RenderableObject::SetGlowmap( const Texture& glowmap )
{
	this->glowmap = glowmap;
}

std::vector<Vertex> RenderableObject::GetWorldspaceVertices()
{
	std::vector<Vertex> worldspace_vertices;
	std::vector<Vertex> objectspace_vertices = GetVertexBuffer().GetVertices();
	for( unsigned int i = 0; i < objectspace_vertices.size(); i++ )
	{
		Vertex worldspace_vertex = objectspace_vertices[i];
		worldspace_vertex.position = (world_transform * GeoVector( worldspace_vertex.position, 1 )).ToGeoFloat3();
		worldspace_vertices.push_back( worldspace_vertex );
	}
	return worldspace_vertices;
}

GeoVector RenderableObject::GetWorldspaceCentroid()
{
	GeoVector centroid( 0, 0, 0 );
	std::vector<Vertex> worldspace_vertices = GetWorldspaceVertices();
	for( unsigned int i = 0; i < worldspace_vertices.size(); i++ )
		centroid += GeoVector( worldspace_vertices[i].position ) / (float)worldspace_vertices.size();
	return centroid;
}