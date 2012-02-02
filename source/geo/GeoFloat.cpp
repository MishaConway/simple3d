#include "GeoFloat.h"

GeoFloat2::GeoFloat2()
{
	x = y = 0;
}

GeoFloat2::GeoFloat2( const float x, const float y )
{
	this->x = x;
	this->y = y;
}

GeoFloat2::GeoFloat2( const unsigned int x, const unsigned int y )
{
	this->x = (float) x;
	this->y = (float) y;
}

GeoFloat3::GeoFloat3()
{
	x = y = z = 0;
}

GeoFloat3::GeoFloat3( const float x, const float y, const float z )
{
	this->x = x;
	this->y = y;
	this->z = z;
}

GeoFloat4::GeoFloat4()
{
	x = y = z = w = 0;
}

GeoFloat4::GeoFloat4( const float x, const float y, const float z, const float w )
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}