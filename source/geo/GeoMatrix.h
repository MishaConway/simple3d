#pragma once

#include "GeoVector.h"

/* 4x4 Matrix */
struct GeoMatrix
{
	float _11, _12, _13, _14;
	float _21, _22, _23, _24;
	float _31, _32, _33, _34;
	float _41, _42, _43, _44;

	/* CONSTRUCTORS */
	GeoMatrix();
    GeoMatrix( const float* );
    GeoMatrix(  float _11, float _12, float _13, float _14,
                float _21, float _22, float _23, float _24,
                float _31, float _32, float _33, float _34,
                float _41, float _42, float _43, float _44 );
#ifdef _WIN32	
	GeoMatrix( const XMMATRIX& m );
	GeoMatrix( const XMFLOAT4X4& m );
#endif

    /* ACCESS GRANTS */
    float& operator () ( unsigned int Row, unsigned int Col );
    float  operator () ( unsigned int Row, unsigned int Col ) const;

	

    /* CASTING OPERATORS */
    //operator float* ();
    //operator const float* () const;

    /* ASSIGNMENT OPERATORS */
    GeoMatrix& operator *= ( const GeoMatrix& );
    GeoMatrix& operator += ( const GeoMatrix& );
    GeoMatrix& operator -= ( const GeoMatrix& );
    GeoMatrix& operator *= ( float );
    GeoMatrix& operator /= ( float );

    /* UNARY OPERATORS */
    GeoMatrix operator + () const;
    GeoMatrix operator - () const;

    /* BINARY OPERATORS */
    GeoMatrix operator * ( const GeoMatrix& ) const;
    GeoMatrix operator + ( const GeoMatrix& ) const;
    GeoMatrix operator - ( const GeoMatrix& ) const;
    GeoMatrix operator * ( float ) const;
    GeoMatrix operator / ( float ) const;
	GeoVector operator * ( const GeoVector& );
    friend GeoMatrix operator * ( float, const GeoMatrix& );

    /* COMPARISON OPERATORS */
	bool operator == ( const GeoMatrix& ) const;
    bool operator != ( const GeoMatrix& ) const; 

#ifdef _WIN32	
	XMMATRIX ToXMMATRIX();
	XMFLOAT4X4 ToXMFLOAT4X4();
#endif

	GeoMatrix Inverse( float* pOutDeterminant );
	GeoMatrix Transpose();

	/* IDENTITY CONSTRUCTOR */
	static GeoMatrix Identity();

	/* PROJECTION MATRIX CONSTRUCTORS */
	static GeoMatrix MatrixPerspectiveFovRH( const float fovy, const float Aspect, const float zn, const float zf );
	static GeoMatrix MatrixPerspectiveFovLH( const float fovy, const float Aspect, const float zn, const float zf );

	/* VIEW MATRIX CONSTRUCTORS */
	static GeoMatrix LookAtRH( const GeoVector& eye_position, const GeoVector& look_at_position, const GeoVector& up_direction );
	static GeoMatrix LookAtLH( const GeoVector& eye_position, const GeoVector& look_at_position, const GeoVector& up_direction );

	/* REFLECTION CONSTRUCTOR */
	static GeoMatrix Reflection( GeoVector reflection_plane );

	/* TRANSLATION CONSTRUCTOR */
	static GeoMatrix Translation( const float x, const float y, const float z );

	/* SCALING CONSTRUCTOR */
	static GeoMatrix Scaling( const float x, const float y, const float z );
	static GeoMatrix Scaling( const float scale );

	/* ROTATION CONSTRUCTORS */
	static GeoMatrix RotationYRH( const float angle );

	GeoVector TransformCoord( const GeoVector& vec );
	GeoVector Transform( const GeoVector& vec );
};