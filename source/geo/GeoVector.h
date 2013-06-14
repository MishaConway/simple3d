#pragma once

#ifdef _WIN32
#define NOMINMAX
#include <d3d11.h>
#include <xnamath.h>
#endif

#include "GeoFloat.h"
#include "../interfaces/IStringable.h"

/* Four component vector */
struct GeoVector : public IStringable
{
	float x, y, z, w;

	/* CONSTRUCTORS */
	GeoVector();
    GeoVector( const float* );
	GeoVector( const float x, const float y );
	GeoVector( const float x, const float y, const float z );
    GeoVector( const float x, const float y, const float z, const float w );
	GeoVector( const GeoFloat3& f );
	GeoVector( const GeoVector& v, const float w );
#ifdef _WIN32
	GeoVector( XMVECTOR vec );
	GeoVector( const XMFLOAT3& vec );
	GeoVector( const XMFLOAT4& vec );
#endif

    /* CASTING OPERATORS */
    operator float* ();
    operator const float* () const;

    /* ASSIGNMENT OPERATORS */
    GeoVector& operator += ( const GeoVector& );
    GeoVector& operator -= ( const GeoVector& );
    GeoVector& operator *= ( float );
    GeoVector& operator /= ( float );

    /* UNARY OPERATORS */
    GeoVector operator + () const;
    GeoVector operator - () const;

    /* BINARY OPERATORS */
    GeoVector operator + ( const GeoVector& ) const;
    GeoVector operator - ( const GeoVector& ) const;
	GeoVector operator * (const GeoVector& ) const;
    GeoVector operator * ( float ) const;
    GeoVector operator / ( float ) const;
    friend GeoVector operator * ( float, const GeoVector& );

    /* COMPARISON OPERATORS */
	bool operator == ( const GeoVector& ) const;
    bool operator != ( const GeoVector& ) const;

	GeoVector Normalize();
	GeoVector Cross( const GeoVector& vec );
	float Dot( const GeoVector& vec ) const;
	float Length();
	float LengthSquared();
	GeoVector Lerp( const GeoVector& vec, const float s );
    static bool IntersectionWithPlane( const GeoVector& p1, const GeoVector& p2, const GeoVector& plane_normal, const GeoVector& point_on_plane, GeoVector* pOutIntersection );
    
    GeoVector InvertXY();

	GeoFloat3 ToGeoFloat3();
    
    virtual std::string ToString();

#ifdef _WIN32	
	XMVECTOR ToXMVec3();
	XMVECTOR ToXMVec4();
	XMFLOAT3 ToXMFloat3();
#endif
};















