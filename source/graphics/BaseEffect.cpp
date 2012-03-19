#include "BaseEffect.h"

BaseEffect::BaseEffect()
{
}

bool BaseEffect::SetColor( const std::string& variable_name, Color color )
{
    SetFloatArray( variable_name, GeoVector( ((float)color.r) / 255.0f, ((float)color.g) / 255.0f, ((float)color.b) / 255.0f, ((float)color.a) / 255.0f ) );
    return true;
}

