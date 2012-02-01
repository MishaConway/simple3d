#include "Color.h"
#include <algorithm>

Color::Color()
{
	r = g = b = 0;
	a = 255;
}

Color::Color( const unsigned int hex )
{
	r = (hex >> 16) & 0xFF;
	g = (hex >>8 ) & 0xFF;
	b = hex & 0xFF;
	a = (hex >> 24) & 0xFF;
}

Color::Color( const unsigned char red, const unsigned char green, const unsigned char blue )
{
	r = red;
	g = green;
	b = blue;
	a = 255;
}

Color::Color( const unsigned char red, const unsigned char green, const unsigned char blue, const unsigned char alpha )
{
	r = red;
	g = green;
	b = blue;
	a = alpha;
}

Color Color::FromFloats( const float red, const float green, const float blue )
{
	return Color( (unsigned char)(255.0f * red),  (unsigned char)(255.0f * green),  (unsigned char)(255.0f * blue) );
}

Color Color::FromFloats( const float red, const float green, const float blue, const float alpha )
{
	return Color( (unsigned char)(255.0f * red),  (unsigned char)(255.0f * green),  (unsigned char)(255.0f * blue), (unsigned char)(255.0f * alpha)  );
}

float Color::GetNormalizedRed()
{
	return (float) r / 255.0f;
}

float Color::GetNormalizedGreen()
{
	return (float) g / 255.0f;
}

float Color::GetNormalizedBlue()
{
	return (float) b / 255.0f;
}

float Color::GetNormalizedAlpha()
{
	return (float) a / 255.0f;
}


// Retrieves the unnormalized max component in the [0,255] range
unsigned char Color::GetMaxComponent()
{
	return std::max( std::max( r, g ), b );
}

// Retrieves the unnormalized min component in the [0,255] range
unsigned char Color::GetMinComponent()
{
	return std::min( std::min( r, g ), b );
}

float Color::GetLuminance()
{
	return (0.27f * (float)r + 0.67f * (float)g + 0.06f * (float)b) / 255.0f;
}

double Color::GetHue()
{
	const double RedComponent   = (double)r;
	const double GreenComponent = (double)g;
	const double BlueComponent  = (double)b;

	const double MinComponent = (double)std::min( std::min(RedComponent, GreenComponent), BlueComponent );
	const double MaxComponent = (double)std::max( std::max(RedComponent, GreenComponent), BlueComponent );
	const double MaxMinusMinNormalized = (MaxComponent - MinComponent ) / 255.0;

	double Hue = 0;
	if( MaxComponent == RedComponent )
	{
		const double GreenMinusBlueNormalized = (GreenComponent - BlueComponent) / 255.0;
		Hue = fmod( 60.0 * GreenMinusBlueNormalized / MaxMinusMinNormalized + 360.0, 360.0 );
	}
	else
	if( MaxComponent == GreenComponent )
	{
		const double BlueMinusRedNormalized = (BlueComponent - RedComponent) / 255.0;
		Hue = ( 60.0 * BlueMinusRedNormalized / MaxMinusMinNormalized + 120.0 );
	}
	else
	if( MaxComponent == BlueComponent )
	{
		const double RedMinusGreenNormalized = (RedComponent - GreenComponent) / 255.0;
		Hue = ( 60.0 * RedMinusGreenNormalized / MaxMinusMinNormalized + 240.0 );
	}
 
	return Hue;
}

double Color::GetSaturation()
{
	const double MaxComponent = (double)GetMaxComponent();
	const double MinComponent = (double)GetMinComponent();
	
	double Saturation = 0;
	if( MaxComponent != MinComponent )
	{
		const double Brightness = GetBrightness();
		const double MaxMinusMinNormalized  = (MaxComponent - MinComponent) / 255.0;
		
		if( Brightness <= 0.5 )
			Saturation = (MaxMinusMinNormalized / (2.0 * Brightness ));
		else
			Saturation = (MaxMinusMinNormalized / (2.0 - 2.0 * Brightness ));
	}

	return Saturation;
}

double Color::GetBrightness()
{
	return 0.5 * ( (double)GetMaxComponent() + (double)GetMinComponent() ) / 255.0;
}

Color Color::FromHueSaturationBrightness( const double Hue, const double Saturation, const double Brightness )
{
    // default our color to gray
	double Red = Brightness, Green = Brightness, Blue = Brightness;
    
    double v = (Brightness <= 0.5) ? (Brightness * (1.0 + Saturation)) : (Brightness + Saturation - Brightness * Saturation );
    if (v > 0)
    {
        double m =  2.0 * Brightness - v;
        double sv = (v - m) / v;

        double h = Hue / (360.0 / 6.0 );
        int sextant = (int)h;
        double fract = h - sextant;

        double vsf = v * sv * fract;
        double mid1 = m + vsf;
        double mid2 = v - vsf;

        switch (sextant)
        {
            case 6:
			case 0:
                Red   = v;
                Green = mid1;
                Blue  = m;
                break;

            case 1:
                Red   = mid2;
                Green = v;
                Blue  = m;
                break;

            case 2:
                Red   = m;
                Green = v;
                Blue  = mid1;
                break;

            case 3:
                Red   = m;
                Green = mid2;
                Blue  = v;
                break;

            case 4:
                Red   = mid1;
                Green = m;
                Blue  = v;
                break;

            case 5:
                Red   = v;
                Green = m;
                Blue  = mid2;
                break;
        }

    }

    // finally return our color rounded and unnormalized to the [0,255] range
	Color c;
	c.r = (unsigned char)((float)Red * 255.0f);
	c.g = (unsigned char)((float)Green * 255.0f);
	c.b = (unsigned char)((float)Blue * 255.0f);
	c.a = 255;
	return c;
}

Color Color::AliceBlue(){ return Color( 0xFFF0F8FF ); }
Color Color::AntiqueWhite(){ return Color(  0xFFFAEBD7); }
Color Color::Aqua(){  return Color(  0xFF00FFFF); }
Color Color::Aquamarine(){  return Color(  0xFF7FFFD4); }
Color Color::Azure(){  return Color(  0xFFF0FFFF); }

Color Color::Beige(){  return Color(  0xFFF5F5DC); }
Color Color::Bisque(){  return Color(  0xFFFFE4C4); }
Color Color::BlanchedAlmond(){  return Color(  0xFFFFEBCD); }
Color Color::BlueViolet(){  return Color(  0xFF8A2BE2); }
Color Color::Brown(){  return Color(  0xFFA52A2A); }
Color Color::BurlyWood(){  return Color(  0xFFDEB887); }

Color Color::CadetBlue(){  return Color(  0xFF5F9EA0); }
Color Color::Chartreuse(){  return Color(  0xFF7FFF00); }
Color Color::Chocolate(){  return Color(  0xFFD2691E); }
Color Color::Coral(){  return Color(  0xFFFF7F50); }
Color Color::CornflowerBlue(){  return Color(  0xFF6495ED); }
Color Color::Cornsilk(){  return Color(  0xFFFFF8DC); }
Color Color::Crimson(){  return Color(  0xFFDC143C); }
Color Color::Cyan(){  return Color(  0xFF00FFFF); }

Color Color::DarkBlue(){  return Color(  0xFF00008B); }
Color Color::DarkCyan(){  return Color(  0xFF008B8B); }
Color Color::DarkGoldenrod(){  return Color(  0xFFB8860B); }
Color Color::DarkGray(){  return Color(  0xFFA9A9A9); }
Color Color::DarkGreen(){  return Color(  0xFF006400); }
Color Color::DarkKhaki(){  return Color(  0xFFBDB76B); }
Color Color::DarkMagenta(){  return Color(  0xFF8B008B); }
Color Color::DarkOliveGreen(){  return Color(  0xFF556B2F); }
Color Color::DarkOrange(){  return Color(  0xFFFF8C00); }
Color Color::DarkOrchid(){  return Color(  0xFF9932CC); }
Color Color::DarkRed(){  return Color(  0xFF8B0000); }
Color Color::DarkSalmon(){  return Color(  0xFFE9967A); }
Color Color::DarkSeaGreen(){  return Color(  0xFF8FBC8F); }
Color Color::DarkSlateBlue(){  return Color(  0xFF483D8B); }
Color Color::DarkSlateGray(){  return Color(  0xFF2F4F4F); }
Color Color::DarkTurquoise(){  return Color(  0xFF00CED1); }
Color Color::DarkViolet(){  return Color(  0xFF9400D3); }
Color Color::DeepPink(){  return Color(  0xFFFF1493); }
Color Color::DeepSkyBlue(){  return Color(  0xFF00BFFF); }
Color Color::DimGray(){  return Color(  0xFF696969); }
Color Color::DodgerBlue(){  return Color(  0xFF1E90FF); }

Color Color::Firebrick(){  return Color(  0xFFB22222); }
Color Color::FloralWhite(){  return Color(  0xFFFFFAF0); }
Color Color::ForestGreen(){  return Color(  0xFF228B22); }
Color Color::Fuchsia(){  return Color(  0xFFFF00FF); }

Color Color::Gainsboro(){  return Color(  0xFFDCDCDC); }
Color Color::GhostWhite(){  return Color(  0xFFF8F8FF); }
Color Color::Gold(){  return Color(  0xFFFFD700); }
Color Color::Goldenrod(){  return Color(  0xFFDAA520); }
Color Color::Gray(){  return Color(  0xFF808080); }
Color Color::GreenYellow(){  return Color(  0xFFADFF2F); }

Color Color::Honeydew(){  return Color(  0xFFF0FFF0); }
Color Color::HotPink(){  return Color(  0xFFFF69B4); }
Color Color::IndianRed(){  return Color(  0xFFCD5C5C); }
Color Color::Indigo(){  return Color(  0xFF4B0082); }
Color Color::Ivory(){  return Color(  0xFFFFFFF0); }
Color Color::Khaki(){  return Color(  0xFFF0E68C); }

Color Color::Lavender(){  return Color(  0xFFE6E6FA); }
Color Color::LavenderBlush(){  return Color(  0xFFFFF0F5); }
Color Color::LawnGreen(){  return Color(  0xFF7CFC00); }
Color Color::LemonChiffon(){  return Color(  0xFFFFFACD); }
Color Color::LightBlue(){  return Color(  0xFFADD8E6); }
Color Color::LightCoral(){  return Color(  0xFFF08080); }
Color Color::LightCyan(){  return Color(  0xFFE0FFFF); }
Color Color::LightGoldenrodYellow(){  return Color(  0xFFFAFAD2); }
Color Color::LightGray(){  return Color(  0xFFD3D3D3); }
Color Color::LightGreen(){  return Color(  0xFF90EE90); }
Color Color::LightPink(){  return Color(  0xFFFFB6C1); }
Color Color::LightSalmon(){  return Color(  0xFFFFA07A); }
Color Color::LightSeaGreen(){  return Color(  0xFF20B2AA); }
Color Color::LightSkyBlue(){  return Color(  0xFF87CEFA); }
Color Color::LightSlateGray(){  return Color(  0xFF778899); }
Color Color::LightSteelBlue(){  return Color(  0xFFB0C4DE); }
Color Color::LightYellow(){  return Color(  0xFFFFFFE0); }
Color Color::Lime(){  return Color(  0xFF00FF00); }
Color Color::LimeGreen(){  return Color(  0xFF32CD32); }
Color Color::Linen(){  return Color(  0xFFFAF0E6); }

Color Color::Magenta(){  return Color(  0xFFFF00FF); }
Color Color::Maroon(){  return Color(  0xFF800000); }
Color Color::MediumAquamarine(){  return Color(  0xFF66CDAA); }
Color Color::MediumBlue(){  return Color(  0xFF0000CD); }
Color Color::MediumOrchid(){  return Color(  0xFFBA55D3); }
Color Color::MediumPurple(){  return Color(  0xFF9370DB); }
Color Color::MediumSeaGreen(){  return Color(  0xFF3CB371); }
Color Color::MediumSlateBlue(){  return Color(  0xFF7B68EE); }
Color Color::MediumSpringGreen(){  return Color(  0xFF00FA9A); }
Color Color::MediumTurquoise(){  return Color(  0xFF48D1CC); }
Color Color::MediumVioletRed(){  return Color(  0xFFC71585); }
Color Color::MidnightBlue(){  return Color(  0xFF191970); }
Color Color::MintCream(){  return Color(  0xFFF5FFFA); }
Color Color::MistyRose(){  return Color(  0xFFFFE4E1); }
Color Color::Moccasin(){  return Color(  0xFFFFE4B5); }

Color Color::NavajoWhite(){  return Color(  0xFFFFDEAD); }
Color Color::Navy(){  return Color(  0xFF000080); }

Color Color::OldLace(){  return Color(  0xFFFDF5E6); }
Color Color::Olive(){  return Color(  0xFF808000); }
Color Color::OliveDrab(){  return Color(  0xFF6B8E23); }
Color Color::Orange(){  return Color(  0xFFFFA500); }
Color Color::OrangeRed(){  return Color(  0xFFFF4500); }
Color Color::Orchid(){  return Color(  0xFFDA70D6); }

Color Color::PaleGoldenrod(){  return Color(  0xFFEEE8AA); }
Color Color::PaleGreen(){  return Color(  0xFF98FB98); }
Color Color::PaleTurquoise(){  return Color(  0xFFAFEEEE); }
Color Color::PaleVioletRed(){  return Color(  0xFFDB7093); }
Color Color::PapayaWhip(){  return Color(  0xFFFFEFD5); }
Color Color::PeachPuff(){  return Color(  0xFFFFDAB9); }
Color Color::Peru(){  return Color(  0xFFCD853F); }
Color Color::Pink(){  return Color(  0xFFFFC0CB); }
Color Color::Plum(){  return Color(  0xFFDDA0DD); }
Color Color::PowderBlue(){  return Color(  0xFFB0E0E6); }
Color Color::Purple(){  return Color(  0xFF800080); }

Color Color::RosyBrown(){  return Color(  0xFFBC8F8F); }
Color Color::RoyalBlue(){  return Color(  0xFF4169E1); }

Color Color::SaddleBrown(){  return Color(  0xFF8B4513); }
Color Color::Salmon(){  return Color(  0xFFFA8072); }
Color Color::SandyBrown(){  return Color(  0xFFF4A460); }
Color Color::SeaGreen(){  return Color(  0xFF2E8B57); }
Color Color::SeaShell(){  return Color(  0xFFFFF5EE); }
Color Color::Sienna(){  return Color(  0xFFA0522D); }
Color Color::Silver(){  return Color(  0xFFC0C0C0); }
Color Color::SkyBlue(){  return Color(  0xFF87CEEB); }
Color Color::SlateBlue(){  return Color(  0xFF6A5ACD); }
Color Color::SlateGray(){  return Color(  0xFF708090); }
Color Color::Snow(){  return Color(  0xFFFFFAFA); }
Color Color::SpringGreen(){  return Color(  0xFF00FF7F); }
Color Color::SteelBlue(){  return Color(  0xFF4682B4); }

Color Color::Tan(){  return Color(  0xFFD2B48C); }
Color Color::Teal(){  return Color(  0xFF008080); }
Color Color::Thistle(){  return Color(  0xFFD8BFD8); }
Color Color::Tomato(){  return Color(  0xFFFF6347); }
Color Color::Turquoise(){  return Color(  0xFF40E0D0); }

Color Color::Violet(){  return Color(  0xFFEE82EE); }
Color Color::Wheat(){  return Color(  0xFFF5DEB3); }
Color Color::WhiteSmoke(){  return Color(  0xFFF5F5F5); }
Color Color::Yellow(){  return Color(  0xFFFFFF00); }
Color Color::YellowGreen(){  return Color(  0xFF9ACD32); }

Color Color::Black(){ return Color( 0, 0, 0 ); }
Color Color::White(){ return Color( 255, 255, 255 ); }
Color Color::Red(){ return Color( 255, 0, 0 ); }
Color Color::Green(){ return Color( 0, 255, 0 ); }
Color Color::Blue(){ return Color( 0, 0, 255 ); }
Color Color::Transparent(){ return Color( 0); }
