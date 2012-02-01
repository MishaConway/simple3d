#pragma once

struct Color
{
	unsigned char r, g, b, a;
	
	Color();
	Color( const unsigned int hex );
	Color( const unsigned char red, const unsigned char green, const unsigned char blue );
	Color( const unsigned char red, const unsigned char green, const unsigned char blue, const unsigned char alpha );

	static Color FromFloats( const float red, const float green, const float blue );
	static Color FromFloats( const float red, const float green, const float blue, const float alpha );

	float GetNormalizedRed();
	float GetNormalizedGreen();
	float GetNormalizedBlue();
	float GetNormalizedAlpha();

	// Compares all components of two colors
	//bool Compare( Color color );
	// Compares only the rgb components of two colors
	//bool CompareRGB( Color color );

	// Retrieves the unnormalized max component in the [0,255] range
	unsigned char GetMaxComponent();
	// Retrieves the unnormalized min component in the [0,255] range
	unsigned char GetMinComponent();

	float GetLuminance();

	// Retrieves the HSB Hue in degree range [0, 360]
	double GetHue();
	// Retrieves the saturation in the range [0, 1]
	double GetSaturation();
	// Retrieves the brightness in the range [0, 1]
	double GetBrightness();

	// Builds an Color given hue [0,360], saturation[0, 1], and brightness[0, 1]
	static Color FromHueSaturationBrightness( const double Hue, const double Saturation, const double Brightess );

	// The following methods retrieve predefined colors
	static Color AliceBlue();
	static Color AntiqueWhite();
	static Color Aqua();
	static Color Aquamarine();
	static Color Azure();
	static Color Beige();
	static Color Bisque();
	static Color BlanchedAlmond();
	static Color BlueViolet();
	static Color Brown();
	static Color BurlyWood();
	static Color CadetBlue();
	static Color Chartreuse();
	static Color Chocolate();
	static Color Coral();
	static Color CornflowerBlue();
	static Color Cornsilk();
	static Color Crimson();
	static Color Cyan();
	static Color DarkBlue();
	static Color DarkCyan();
	static Color DarkGoldenrod();
	static Color DarkGray();
	static Color DarkGreen();
	static Color DarkKhaki();
	static Color DarkMagenta();
	static Color DarkOliveGreen();
	static Color DarkOrange();
	static Color DarkOrchid();
	static Color DarkRed();
	static Color DarkSalmon();
	static Color DarkSeaGreen();
	static Color DarkSlateBlue();
	static Color DarkSlateGray();
	static Color DarkTurquoise();
	static Color DarkViolet();
	static Color DeepPink();
	static Color DeepSkyBlue();
	static Color DimGray();
	static Color DodgerBlue();
	static Color Firebrick();
	static Color FloralWhite();
	static Color ForestGreen();
	static Color Fuchsia();
	static Color Gainsboro();
	static Color GhostWhite();
	static Color Gold();
	static Color Goldenrod();
	static Color Gray();
	static Color GreenYellow();
	static Color Honeydew();
	static Color HotPink();
	static Color IndianRed();
	static Color Indigo();
	static Color Ivory();
	static Color Khaki();
	static Color Lavender();
	static Color LavenderBlush();
	static Color LawnGreen();
	static Color LemonChiffon();
	static Color LightBlue();
	static Color LightCoral();
	static Color LightCyan();
	static Color LightGoldenrodYellow();
	static Color LightGray();
	static Color LightGreen();
	static Color LightPink();
	static Color LightSalmon();
	static Color LightSeaGreen();
	static Color LightSkyBlue();
	static Color LightSlateGray();
	static Color LightSteelBlue();
	static Color LightYellow();
	static Color Lime();
	static Color LimeGreen();
	static Color Linen();
	static Color Magenta();
	static Color Maroon();
	static Color MediumAquamarine();
	static Color MediumBlue();
	static Color MediumOrchid();
	static Color MediumPurple();
	static Color MediumSeaGreen();
	static Color MediumSlateBlue();
	static Color MediumSpringGreen();
	static Color MediumTurquoise();
	static Color MediumVioletRed();
	static Color MidnightBlue();
	static Color MintCream();
	static Color MistyRose();
	static Color Moccasin();
	static Color NavajoWhite();
	static Color Navy();
	static Color OldLace();
	static Color Olive();
	static Color OliveDrab();
	static Color Orange();
	static Color OrangeRed();
	static Color Orchid();
	static Color PaleGoldenrod();
	static Color PaleGreen();
	static Color PaleTurquoise();
	static Color PaleVioletRed();
	static Color PapayaWhip();
	static Color PeachPuff();
	static Color Peru();
	static Color Pink();
	static Color Plum();
	static Color PowderBlue();
	static Color Purple();
	static Color RosyBrown();
	static Color RoyalBlue();
	static Color SaddleBrown();
	static Color Salmon();
	static Color SandyBrown();
	static Color SeaGreen();
	static Color SeaShell();
	static Color Sienna();
	static Color Silver();
	static Color SkyBlue();
	static Color SlateBlue();
	static Color SlateGray();
	static Color Snow();
	static Color SpringGreen();
	static Color SteelBlue();
	static Color Tan();
	static Color Teal();
	static Color Thistle();
	static Color Tomato();
	static Color Turquoise();
	static Color Violet();
	static Color Wheat();
	static Color WhiteSmoke();
	static Color Yellow();
	static Color YellowGreen();
	static Color Black();
	static Color White();
	static Color Red();
	static Color Green();
	static Color Blue();
	static Color Transparent();
};