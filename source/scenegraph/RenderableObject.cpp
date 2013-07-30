#include "RenderableObject.h"

std::string RenderableObject::enforced_global_technique;
std::string RenderableObject::root_assets_path;

void RenderableObject::SetDefaultValues()
{
	visible = true;
	planar_reflector = false;
    two_sided = false;
	blend_type = BlendType::NONE;
    color = Color::Brown();
    specular_color = Color::White();
    specular_power = 16;
}

RenderableObject::RenderableObject()
{
	SetDefaultValues();
}

RenderableObject::RenderableObject( Geometry geometry )
{
    SetDefaultValues();
	vertex_buffer = VertexBuffer( geometry.GetVertices() );    
}

RenderableObject::RenderableObject( Texture t, VertexBuffer v) : tex(t), vertex_buffer(v)
{
	SetDefaultValues();
}

RenderableObject::RenderableObject( Texture t, Geometry geometry ) : tex(t)
{
	SetDefaultValues();
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

void RenderableObject::SetTwoSided( const bool two_sided )
{
    this->two_sided = two_sided;
}

void RenderableObject::SetColor( const Color color )
{
    this->color = color;
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
	return "Phong";
}

bool RenderableObject::Render()  //consider passing in whether or not front or back faces should be rendered
{
	if( visible )
	{
		constant_buffer.data.world_transform = GetWorldTransform();
		constant_buffer.data.world_transform_inverse = GetWorldInverseTranspose();
	//	constant_buffer.Update();
		
		
		Effect e = Effect::GetCurrentEffect();
		e.SetTexture( "lala", GetTexture() );
		e.SetTexture( "glowmap", GetGlowmap() );
		e.SetMatrix( "WorldTransform", GetWorldTransform() );
		e.SetMatrix( "WorldInverseTranspose", GetWorldInverseTranspose() );
		e.SetInt( "compute_projective_texture_coordinates", planar_reflector );
        e.SetColor( "color", color);
        e.SetColor( "specular_color", specular_color);
        e.SetFloat("specular_power", specular_power);
		pGraphicsDevice->SetVertexBuffer( GetVertexBuffer() );
		pGraphicsDevice->GetStateManager().SetDefaults();
        if( two_sided )
            pGraphicsDevice->GetStateManager().SetDefaultFrontAndBackRendering();
        
        pGraphicsDevice->GetStateManager().SetBlendType(blend_type);
        
			
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

float RenderableObject::GetHeight() 
{
    std::vector<Vertex> objectspace_vertices = GetVertexBuffer().GetVertices();
    if( objectspace_vertices.empty() )
        return 0;
    
    float min_y = objectspace_vertices[0].position.y;
    float max_y = min_y;
    
    
    for( unsigned int i = 1; i < objectspace_vertices.size(); i++ )
    {
            if( objectspace_vertices[i].position.y > max_y )
                max_y = objectspace_vertices[i].position.y;
        
            if( objectspace_vertices[i].position.y < min_y )
                min_y = objectspace_vertices[i].position.y;
    }
    
    
    
    return max_y - min_y;
}

