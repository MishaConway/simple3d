#include "Sprite.h"

Sprite::Sprite() : Quad(tex)
{
	position.x = position.y = size.x = size.y = 0;
	blend_type = BlendType::ALPHA;
}
Sprite::Sprite( Texture t): Quad(t)  
{
	position.x = position.y = size.x = size.y = 0;	
	blend_type = BlendType::ALPHA;
}

Sprite::Sprite( Texture t, const GeoFloat2& p, const GeoFloat2& s ) : Quad(t)
{
	SetPosition(p);
	SetSize(s);
	blend_type = BlendType::ALPHA;
}

Sprite::Sprite( Texture t, const GeoFloat2& p, const GeoFloat2& s, const BlendType::GRAPHICS_DEVICE_STATE_MANAGER_BLEND_TYPE blend_type ) : Quad(t)
{
	SetPosition(p);
	SetSize(s);
	SetBlendType( blend_type );
}

Sprite::Sprite( Texture t, const GeoFloat2& p, const GeoFloat2& s, const BlendType::GRAPHICS_DEVICE_STATE_MANAGER_BLEND_TYPE blend_type, const std::string& technique ) : Quad(t)
{
	SetPosition(p);
	SetSize(s);
	SetBlendType( blend_type );
	SetTechnique( technique );
}

void Sprite::SetPosition( const GeoFloat2& p )
{
	position = p;
}

void Sprite::SetSize( const GeoFloat2& s )
{
	size = s;
}

GeoFloat2 Sprite::GetSize()
{
	return size;
}

GeoFloat2 Sprite::GetPosition()
{
	return position;
}

void Sprite::SetWidth( const float w )
{
	size.x = w;
}

void Sprite::SetHeight( const float h )
{
	size.y = h;
}

std::string Sprite::GetDefaultTechnique()
{
	return "Sprite";
}

bool Sprite::Render()
{
	if( visible )
	{
		SetIdentity();
		Scale( size.x, size.y, 0 );
		Translate( position.x + size.x / 2.0f, position.y + size.y / 2.0f, 0 );

		Effect e = Effect::GetCurrentEffect();
		e.SetTexture( "lala", GetTexture() );
		e.SetMatrix( "WorldTransform", GetWorldTransform() );
		pGraphicsDevice->SetVertexBuffer( GetVertexBuffer() );
		pGraphicsDevice->GetStateManager().SetSpriteRendering( blend_type );

		#ifdef _WIN32				
		e.RenderTechnique( GetTechnique(), [this](){ pGraphicsDevice->Draw( GetVertexBuffer().GetNumVertices() );});
		#else
		e.SetTechnique( GetTechnique() );
		pGraphicsDevice->Draw( GetVertexBuffer().GetNumVertices() );
		e.UnsetTechnique();
		#endif
	}
	return true;	
}


