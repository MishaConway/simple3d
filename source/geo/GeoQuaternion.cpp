#include "GeoQuaternion.h"
#include <math.h>
#include <stdio.h>

GeoQuaternion::GeoQuaternion()
{
	x = y = z = w = 0;
}

std::string GeoQuaternion::ToString()
{
    char buffer[512];
    GeoVector axis = GetAxis();
    sprintf( buffer, "GeoQuaternion: Axis:(%f, %f, %f) Angle:%f degrees\n", axis.x, axis.y, axis.z, GetAngle() );
    return std::string( buffer );
}

GeoQuaternion::GeoQuaternion( const GeoVector& rotation_axis, const float degrees )
{
	GeoVector normalized_rotation_axis = GeoVector(rotation_axis).Normalize();
	const float radians = GeoConvertToRadians( degrees );
    x = sin( radians / 2.0f ) * normalized_rotation_axis.x;
    y = sin( radians / 2.0f ) * normalized_rotation_axis.y;
    z = sin( radians / 2.0f ) * normalized_rotation_axis.z;
    w = cos( radians / 2.0f );
}
	
GeoQuaternion GeoQuaternion::operator * ( const GeoQuaternion& quat ) const
{    
    GeoQuaternion out;
	out.w = w * quat.w - x * quat.x - y * quat.y - z * quat.z;
	out.x = w * quat.x + x * quat.w + y * quat.z - z * quat.y;
	out.y = w * quat.y - x * quat.z + y * quat.w + z * quat.x;
	out.z = w * quat.z + x * quat.y - y * quat.x + z * quat.w;
        
    return out;
}

GeoMatrix GeoQuaternion::ToMatrix()
{
	GeoVector v = GeoVector( x, y, z, w ); //Normalize();
	GeoMatrix matrix = GeoMatrix::Identity();
    matrix._11 = 1.0f - 2.0f * (v.y * v.y + v.z * v.z);
    matrix._12 = 2.0f * (v.x * v.y + v.z * v.w);
    matrix._13 = 2.0f * (v.x * v.z - v.y * v.w);
    matrix._21 = 2.0f * (v.x * v.y - v.z * v.w);
    matrix._22 = 1.0f - 2.0f * (v.x * v.x + v.z * v.z);
    matrix._23 = 2.0f * (v.y * v.z + v.x * v.w);
    matrix._31 = 2.0f * (v.x * v.z + v.y * v.w);
    matrix._32 = 2.0f * (v.y * v.z - v.x * v.w);
    matrix._33 = 1.0f - 2.0f * (v.x * v.x + v.y * v.y);
    return matrix;
}

GeoVector GeoQuaternion::GetAxis()
{
    return GeoVector( x, y, z ) / sqrtf( x*x + y*y + z*z );
}

float     GeoQuaternion::GetAngle()
{
    return GeoConvertToDegrees( acos(w) * 2.0f );
}