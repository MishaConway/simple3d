#pragma once
#include "GeoVector.h"
#include "GeoMatrix.h"

#define GEO_PI               3.141592654f
#define GEO_2PI              6.283185307f
#define GEO_1DIVPI           0.318309886f
#define GEO_1DIV2PI          0.159154943f
#define GEO_PIDIV2           1.570796327f
#define GEO_PIDIV4           0.785398163f

#define GeoConvertToRadians(X) (X*GEO_PI/180.0f) 
#define GeoConvertToDegrees(radians) ((radians) * (180.0 / GEO_PI))

/* Four component vector */
class GeoQuaternion : public IStringable
{
public:
	GeoQuaternion();
	GeoQuaternion( const GeoVector& rotation_axis, const float degrees );
	GeoQuaternion operator * ( const GeoQuaternion& ) const;
	GeoMatrix ToMatrix();
    GeoVector GetAxis();
    float     GetAngle();
    virtual std::string ToString();
private:
	float w, x, y, z;
};
 
