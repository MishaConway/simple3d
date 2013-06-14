#include "GeoVector.h"
#include <math.h>
#include <stdio.h>

std::string GeoVector::ToString()
{
    char buffer[512];
    sprintf( buffer, "GeoVector:%f, %f, %f, %f\n", x, y, z, w );
    return std::string(buffer);
}

//constructors
GeoVector::GeoVector() 
{
	x = y = z = w = 0;
}

GeoVector::GeoVector( const float* pV )
{
	x = pV[0];
	y = pV[1];
	z = pV[2];
	w = pV[3];
}

GeoVector::GeoVector( const float x, const float y )
{
	this->x = x;
	this->y = y;
	z = w = 0;
}

GeoVector::GeoVector( const float x, const float y, const float z )
{
	this->x = x;
	this->y = y;
	this->z = z;
	w = 0;
}

GeoVector::GeoVector( const float x, const float y, const float z, const float w )
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

GeoVector::GeoVector( const GeoFloat3& vec )
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	w = 0;
}

GeoVector::GeoVector( const GeoVector& v, const float w )
{
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
	this->w = w;
}

#ifdef _WIN32	
GeoVector::GeoVector( XMVECTOR vec )
{
	XMFLOAT3 v;
	XMStoreFloat3( &v, vec );
	x = v.x;
	y = v.y;
	z = v.z;
	w = 0;
}

GeoVector::GeoVector( const XMFLOAT3& vec )
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	w = 0;
}

GeoVector::GeoVector( const XMFLOAT4& vec )
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	w = vec.w;
}
#endif



// assignment operators
GeoVector& GeoVector::operator += ( const GeoVector& vec4 )
{
	x += vec4.x;
	y += vec4.y;
	z += vec4.z;
	w += vec4.w; //unsure about this one
	return *this;
}

GeoVector& GeoVector::operator -= ( const GeoVector& vec4 )
{
	x -= vec4.x;
	y -= vec4.y;
	z -= vec4.z;
	w -= vec4.w; //unsure about this one
	return *this;
}

GeoVector& GeoVector::operator *= ( float scalar )
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	w *= scalar; //unsure about this one
	return *this;
}

GeoVector& GeoVector::operator /= ( float scalar )
{
	x /= scalar;
	y /= scalar;
	z /= scalar;
	w /= scalar; //unsure about this one
	return *this;
}

// unary operators
GeoVector GeoVector::operator + () const
{
	return *this;
}

GeoVector GeoVector::operator - () const
{
	GeoVector vec4;
	vec4.x = -x;
	vec4.y = -y;
	vec4.z = -z;
	vec4.w = -w; //unsure about this one
	return vec4;
}

// binary operators
GeoVector GeoVector::operator + ( const GeoVector& vec4 ) const
{
	GeoVector vecSum;
	vecSum.x = x + vec4.x;
	vecSum.y = y + vec4.y;
	vecSum.z = z + vec4.z;
	vecSum.w = w + vec4.w; //unsure about this one
	return vecSum;
}

GeoVector GeoVector::operator - ( const GeoVector& vec4 ) const
{
	GeoVector vecSum;
	vecSum.x = x - vec4.x;
	vecSum.y = y - vec4.y;
	vecSum.z = z - vec4.z;
	vecSum.w = w - vec4.w; //unsure about this one
	return vecSum;
}

GeoVector GeoVector::operator * ( const GeoVector& vec4 ) const
{
	GeoVector vecProduct;
	vecProduct.x = x * vec4.x;
	vecProduct.y = y * vec4.y;
	vecProduct.z = z * vec4.z;
	vecProduct.w = w * vec4.w; //unsure about this one
	return vecProduct;
}

GeoVector GeoVector::operator * ( float scalar ) const
{
	GeoVector vec4;
	vec4.x = x * scalar;
	vec4.y = y * scalar;
	vec4.z = z * scalar;
	vec4.w = w * scalar;  //unsure about this one
	return vec4;
}

GeoVector GeoVector::operator / ( float scalar ) const
{
	GeoVector vec4;
	vec4.x = x / scalar;
	vec4.y = y / scalar;
	vec4.z = z / scalar;
	vec4.w = w / scalar;
	return vec4;
}

//friend OGLXVECTOR4 OGLXVECTOR4::operator * ( float, const OGLXVECTOR4& );

//comparison operators
bool GeoVector::operator == ( const GeoVector& vec4 ) const
{
	return x == vec4.x && y == vec4.y && z == vec4.z && w == vec4.w;
}

bool GeoVector::operator != ( const GeoVector& vec4 ) const
{
	return x != vec4.x || y != vec4.y || z != vec4.z || w != vec4.w;
}

GeoVector GeoVector::Normalize()
{
	const float inverse_length = 1.0f / Length();
	x *= inverse_length;
	y *= inverse_length;
	z *= inverse_length;
	return *this;
}

GeoVector GeoVector::Cross( const GeoVector& vec )
{
	GeoVector out;
	out.x = y * vec.z - z * vec.y;
	out.y = z * vec.x - x * vec.z;
	out.z = x * vec.y - y * vec.x;
	return out;
}
	
float GeoVector::Dot( const GeoVector& vec ) const
{
	return x * vec.x + y * vec.y + z * vec.z;
	//the w part screws up the camera
}

float GeoVector::Length()
{
	return sqrtf( LengthSquared() );
}

float GeoVector::LengthSquared()
{
	return Dot( *this );
}

GeoVector GeoVector::Lerp( const GeoVector& vec, const float s )
{
	return *this + ( vec - *this )*s;	
}

bool GeoVector::IntersectionWithPlane( const GeoVector& p1, const GeoVector& p2, const GeoVector& plane_normal, const GeoVector& point_on_plane, GeoVector* pOutIntersection )
{    
    const float u_denom = plane_normal.Dot( p2 - p1 );
    if( !u_denom )
        return false;
    const float u_numerator = plane_normal.Dot( point_on_plane - p1 );
    const float u = u_numerator / u_denom;    
    if( pOutIntersection )
        *pOutIntersection = p1 + (p2 - p1) * u;
    
    return true;
}

GeoVector GeoVector::InvertXY()
{
    return GeoVector( y, x, z, w );
}

GeoFloat3 GeoVector::ToGeoFloat3()
{
	return GeoFloat3( x, y, z );
}

#ifdef _WIN32	
XMVECTOR GeoVector::ToXMVec3()
{
	XMFLOAT3 v( x, y, z );
	return XMLoadFloat3( &v );
}

XMVECTOR GeoVector::ToXMVec4()
{
	XMFLOAT4 v( x, y, z, w );
	return XMLoadFloat4( &v );
}

XMFLOAT3 GeoVector::ToXMFloat3()
{
	return XMFLOAT3( x, y, z );
}
#endif
