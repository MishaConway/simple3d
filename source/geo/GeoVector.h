#pragma once

#ifdef _WIN32
#include <d3d11.h>
#include <xnamath.h>
#endif

/* Four component vector */
struct GeoVector
{
	float x, y, z, w;

	/* CONSTRUCTORS */
	GeoVector();
    GeoVector( const float* );
	GeoVector( const float x, const float y );
	GeoVector( const float x, const float y, const float z );
    GeoVector( const float x, const float y, const float z, const float w );
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
    GeoVector operator * ( float ) const;
    GeoVector operator / ( float ) const;
    friend GeoVector operator * ( float, const GeoVector& );

    /* COMPARISON OPERATORS */
	bool operator == ( const GeoVector& ) const;
    bool operator != ( const GeoVector& ) const;

	GeoVector Normalize();
	GeoVector Cross( const GeoVector& vec );
	float Dot( const GeoVector& vec );
	float Length();
	float LengthSquared();
	GeoVector Lerp( const GeoVector& vec, const float s ); 

#ifdef _WIN32	
	XMVECTOR ToXMVec3();
	XMVECTOR ToXMVec4();
	XMFLOAT3 ToXMFloat3();
#endif
};



