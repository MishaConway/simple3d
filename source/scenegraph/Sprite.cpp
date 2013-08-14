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
    SetIdentity();
    Scale( size.x, size.y, 0 );
    Translate( position.x, position.y, 0 );
}

void Sprite::SetSize( const GeoFloat2& s )
{
	size = s;
    SetIdentity();
    Scale( size.x, size.y, 0 );
    Translate( position.x, position.y, 0 );
    
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
		Effect& e = Effect::GetCurrentEffect();
		e.SetTexture( "lala", GetTexture() );
		e.SetMatrix( "WorldTransform", GetWorldTransform() );
        e.SetColor( "color", color );
		pGraphicsDevice->SetVertexBuffer( GetVertexBuffer() );
		pGraphicsDevice->GetStateManager().SetSpriteRendering( blend_type );

        BEGIN_RENDER_CURRENT_TECHNIQUE(e)
            pGraphicsDevice->Draw( GetVertexBuffer().GetNumVertices() );
        END_RENDER_TECHNIQUE
	}
	return true;	
}


