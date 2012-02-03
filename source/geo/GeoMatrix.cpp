#include "GeoMatrix.h"
#include <math.h>

/* OGLXMATRIX IMPLEMENTATION */
GeoMatrix::GeoMatrix() {}
GeoMatrix::GeoMatrix( const float* pM )
{
	_11 = pM[0];
	_12 = pM[1];
	_13 = pM[2];
	_14 = pM[3];

	_21 = pM[4];
	_22 = pM[5];
	_23 = pM[6];
	_24 = pM[7];

	_31 = pM[8];
	_32 = pM[9];
	_33 = pM[10];
	_34 = pM[11];

	_41 = pM[12];
	_42 = pM[13];
	_43 = pM[14];
	_44 = pM[15];
}

GeoMatrix::GeoMatrix( float _11, float _12, float _13, float _14, float _21, float _22, float _23, float _24,
					  float _31, float _32, float _33, float _34, float _41, float _42, float _43, float _44  )
{
	this->_11 = _11;
	this->_12 = _12;
	this->_13 = _13;
	this->_14 = _14;

	this->_21 = _21;
	this->_22 = _22;
	this->_23 = _23;
	this->_24 = _24;

	this->_31 = _31;
	this->_32 = _32;
	this->_33 = _33;
	this->_34 = _34;

	this->_41 = _41;
	this->_42 = _42;
	this->_43 = _43;
	this->_44 = _44;	
}

#ifdef _WIN32
GeoMatrix::GeoMatrix( const XMMATRIX& matrix )
{
	XMFLOAT4X4 m;
	XMStoreFloat4x4( &m, matrix ); 
	this->_11 = m._11;
	this->_12 = m._12;
	this->_13 = m._13;
	this->_14 = m._14;

	this->_21 = m._21;
	this->_22 = m._22;
	this->_23 = m._23;
	this->_24 = m._24;

	this->_31 = m._31;
	this->_32 = m._32;
	this->_33 = m._33;
	this->_34 = m._34;

	this->_41 = m._41;
	this->_42 = m._42;
	this->_43 = m._43;
	this->_44 = m._44;
}

GeoMatrix::GeoMatrix( const XMFLOAT4X4& m )
{
	this->_11 = m._11;
	this->_12 = m._12;
	this->_13 = m._13;
	this->_14 = m._14;

	this->_21 = m._21;
	this->_22 = m._22;
	this->_23 = m._23;
	this->_24 = m._24;

	this->_31 = m._31;
	this->_32 = m._32;
	this->_33 = m._33;
	this->_34 = m._34;

	this->_41 = m._41;
	this->_42 = m._42;
	this->_43 = m._43;
	this->_44 = m._44;
}
#endif

// access grants
float& GeoMatrix::operator () ( unsigned int Row, unsigned int Col )
{
	return *( &_11 + 4*Row + Col );
}

float  GeoMatrix::operator () ( unsigned int Row, unsigned int Col ) const
{
	return *( &_11 + 4*Row + Col );
}

// casting operators
//OGLXMATRIX::operator float* ();
//OGLXMATRIX::operator const float* () const;

// assignment operators
GeoMatrix& GeoMatrix::operator *= ( const GeoMatrix& matrix )
{
	//GeoMatrix mResult; 
/*
    // Cache the invariants in registers
    float x = _11;
    float y = _12;
    float z = _13;
    float w = _14;
    // Perform the operation on the first row
    mResult._11 = (matrix._11*x)+(matrix._21*y)+(matrix._31*z)+(matrix._41*w);
    mResult._12 = (matrix._12*x)+(matrix._22*y)+(matrix._32*z)+(matrix._42*w);
    mResult._13 = (matrix._13*x)+(matrix._23*y)+(matrix._33*z)+(matrix._43*w);
    mResult._14 = (matrix._14*x)+(matrix._24*y)+(matrix._34*z)+(matrix._44*w);
    // Repeat for all the other rows
    x = _21;
    y = _22;
    z = _23;
    w = _24;
    mResult._21 = (matrix._11*x)+(matrix._21*y)+(matrix._31*z)+(matrix._41*w);
    mResult._22 = (matrix._12*x)+(matrix._22*y)+(matrix._32*z)+(matrix._42*w);
    mResult._23 = (matrix._13*x)+(matrix._23*y)+(matrix._33*z)+(matrix._43*w);
    mResult._24 = (matrix._14*x)+(matrix._24*y)+(matrix._34*z)+(matrix._44*w);
    x = _31;
    y = _32;
    z = _33;
    w = _34;
    mResult._31 = (matrix._11*x)+(matrix._21*y)+(matrix._31*z)+(matrix._41*w);
    mResult._32 = (matrix._12*x)+(matrix._22*y)+(matrix._32*z)+(matrix._42*w);
    mResult._33 = (matrix._13*x)+(matrix._23*y)+(matrix._33*z)+(matrix._43*w);
    mResult._34 = (matrix._14*x)+(matrix._24*y)+(matrix._34*z)+(matrix._44*w);
    x = _41;
    y = _42;
    z = _43;
    w = _44;
    mResult._41 = (matrix._11*x)+(matrix._21*y)+(matrix._31*z)+(matrix._41*w);
    mResult._42 = (matrix._12*x)+(matrix._22*y)+(matrix._32*z)+(matrix._42*w);
    mResult._43 = (matrix._13*x)+(matrix._23*y)+(matrix._33*z)+(matrix._43*w);
    mResult._44 = (matrix._14*x)+(matrix._24*y)+(matrix._34*z)+(matrix._44*w);

	memcpy( this, &mResult, sizeof(mResult ) );

    return mResult;
	*/

	GeoMatrix m = *this;

	//row one
	_11 =	m._11 * matrix._11 + m._12 * matrix._21 + m._13 * matrix._31 + m._14 * matrix._41;
	_12 =	m._11 * matrix._12 + m._12 * matrix._22 + m._13 * matrix._32 + m._14 * matrix._42;
	_13 =	m._11 * matrix._13 + m._12 * matrix._23 + m._13 * matrix._33 + m._14 * matrix._43;
	_14 =	m._11 * matrix._14 + m._12 * matrix._24 + m._13 * matrix._34 + m._14 * matrix._44;

	//row two
	_21 =	m._21 * matrix._11 + m._22 * matrix._21 + m._23 * matrix._31 + m._24 * matrix._41;
	_22 =	m._21 * matrix._12 + m._22 * matrix._22 + m._23 * matrix._32 + m._24 * matrix._42;
	_23 =	m._21 * matrix._13 + m._22 * matrix._23 + m._23 * matrix._33 + m._24 * matrix._43;
	_24 =	m._21 * matrix._14 + m._22 * matrix._24 + m._23 * matrix._34 + m._24 * matrix._44;

	//row three
	_31 =	m._31 * matrix._11 + m._32 * matrix._21 + m._33 * matrix._31 + m._34 * matrix._41;
	_32 =	m._31 * matrix._12 + m._32 * matrix._22 + m._33 * matrix._32 + m._34 * matrix._42;
	_33 =	m._31 * matrix._13 + m._32 * matrix._23 + m._33 * matrix._33 + m._34 * matrix._43;
	_34 =	m._31 * matrix._14 + m._32 * matrix._24 + m._33 * matrix._34 + m._34 * matrix._44;

	//row four
	_41 =	m._41 * matrix._11 + m._42 * matrix._21 + m._43 * matrix._31 + m._44 * matrix._41;
	_42 =	m._41 * matrix._12 + m._42 * matrix._22 + m._43 * matrix._32 + m._44 * matrix._42;
	_43 =	m._41 * matrix._13 + m._42 * matrix._23 + m._43 * matrix._33 + m._44 * matrix._43;
	_44 =	m._41 * matrix._14 + m._42 * matrix._24 + m._43 * matrix._34 + m._44 * matrix._44;
													  
	return *this;
}

GeoMatrix& GeoMatrix::operator += ( const GeoMatrix& matrix )
{
	_11 += matrix._11;
	_12 += matrix._12;
	_13 += matrix._13;
	_14 += matrix._14;

	_21 += matrix._21;
	_22 += matrix._22;
	_23 += matrix._23;
	_24 += matrix._24;

	_31 += matrix._31;
	_32 += matrix._32;
	_33 += matrix._33;
	_34 += matrix._34;

	_41 += matrix._41;
	_42 += matrix._42;
	_43 += matrix._43;
	_44 += matrix._44;

	return *this; 
}

GeoMatrix& GeoMatrix::operator -= ( const GeoMatrix& matrix )
{
	_11 -= matrix._11;
	_12 -= matrix._12;
	_13 -= matrix._13;
	_14 -= matrix._14;

	_21 -= matrix._21;
	_22 -= matrix._22;
	_23 -= matrix._23;
	_24 -= matrix._24;

	_31 -= matrix._31;
	_32 -= matrix._32;
	_33 -= matrix._33;
	_34 -= matrix._34;

	_41 -= matrix._41;
	_42 -= matrix._42;
	_43 -= matrix._43;
	_44 -= matrix._44;

	return *this;
}

GeoMatrix& GeoMatrix::operator *= ( float scalar )
{
	_11 *= scalar;
	_12 *= scalar;
	_13 *= scalar;
	_14 *= scalar;

	_21 *= scalar;
	_22 *= scalar;
	_23 *= scalar;
	_24 *= scalar;

	_31 *= scalar;
	_32 *= scalar;
	_33 *= scalar;
	_34 *= scalar;

	_41 *= scalar;
	_42 *= scalar;
	_43 *= scalar;
	_44 *= scalar;

	return *this;
}

GeoMatrix& GeoMatrix::operator /= ( float scalar )
{
	_11 /= scalar;
	_12 /= scalar;
	_13 /= scalar;
	_14 /= scalar;

	_21 /= scalar;
	_22 /= scalar;
	_23 /= scalar;
	_24 /= scalar;

	_31 /= scalar;
	_32 /= scalar;
	_33 /= scalar;
	_34 /= scalar;

	_41 /= scalar;
	_42 /= scalar;
	_43 /= scalar;
	_44 /= scalar;

	return *this;
}

// unary operators
GeoMatrix GeoMatrix::operator + () const
{
	return *this;
}

GeoMatrix GeoMatrix::operator - () const
{
	GeoMatrix matrix;

	matrix._11 = -_11;
	matrix._12 = -_12;
	matrix._13 = -_13;
	matrix._14 = -_14;

	matrix._21 = -_21;
	matrix._22 = -_22;
	matrix._23 = -_23;
	matrix._24 = -_24;

	matrix._31 = -_31;
	matrix._32 = -_32;
	matrix._33 = -_33;
	matrix._34 = -_34;

	matrix._41 = -_41;
	matrix._42 = -_42;
	matrix._43 = -_43;
	matrix._44 = -_44;

	return matrix;
}

// binary operators
GeoMatrix GeoMatrix::operator * ( const GeoMatrix& matrix ) const
{
	GeoMatrix matrixProduct( (float*) this );
	matrixProduct *= matrix;
	return matrixProduct;
}

GeoVector GeoMatrix::operator * ( const GeoVector& vec )
{	
	GeoVector transformed_vector;
	transformed_vector.x = _11 * vec.x + _21 * vec.y + _31 * vec.z + _41;
	transformed_vector.y = _12 * vec.x + _22 * vec.y + _32 * vec.z + _42;
	transformed_vector.z = _13 * vec.x + _23 * vec.y + _33 * vec.z + _43;
	transformed_vector.w = _14 * vec.x + _24 * vec.y + _34 * vec.z + _44;
	return transformed_vector;
}

GeoVector GeoMatrix::TransformCoord( const GeoVector& vec )
{
	const float norm =_14 * vec.x + _24 * vec.y + _34 * vec.z + _44;
	GeoVector transformed_vector = ((*this) * vec);
	transformed_vector.x /= norm;
	transformed_vector.y /= norm;
	transformed_vector.z /= norm;
	return transformed_vector;
}
	
GeoVector GeoMatrix::Transform( const GeoVector& vec )
{
	return (*this) * vec;
}

GeoMatrix GeoMatrix::operator + ( const GeoMatrix& matrix ) const
{
	GeoMatrix matrixSum;

	matrixSum._11 = _11 + matrix._11;
	matrixSum._12 = _12 + matrix._12;
	matrixSum._13 = _13 + matrix._13;
	matrixSum._14 = _14 + matrix._14;

	matrixSum._21 = _21 + matrix._21;
	matrixSum._22 = _22 + matrix._22;
	matrixSum._23 = _23 + matrix._23;
	matrixSum._24 = _24 + matrix._24;

	matrixSum._31 = _31 + matrix._31;
	matrixSum._32 = _32 + matrix._32;
	matrixSum._33 = _33 + matrix._33;
	matrixSum._34 = _34 + matrix._34;

	matrixSum._41 = _41 + matrix._41;
	matrixSum._42 = _42 + matrix._42;
	matrixSum._43 = _43 + matrix._43;
	matrixSum._44 = _44 + matrix._44;

	return matrixSum;
}

GeoMatrix GeoMatrix::operator - ( const GeoMatrix& matrix ) const
{
	GeoMatrix matrixSum;

	matrixSum._11 = _11 - matrix._11;
	matrixSum._12 = _12 - matrix._12;
	matrixSum._13 = _13 - matrix._13;
	matrixSum._14 = _14 - matrix._14;

	matrixSum._21 = _21 - matrix._21;
	matrixSum._22 = _22 - matrix._22;
	matrixSum._23 = _23 - matrix._23;
	matrixSum._24 = _24 - matrix._24;

	matrixSum._31 = _31 - matrix._31;
	matrixSum._32 = _32 - matrix._32;
	matrixSum._33 = _33 - matrix._33;
	matrixSum._34 = _34 - matrix._34;

	matrixSum._41 = _41 - matrix._41;
	matrixSum._42 = _42 - matrix._42;
	matrixSum._43 = _43 - matrix._43;
	matrixSum._44 = _44 - matrix._44;

	return matrixSum;
}

GeoMatrix GeoMatrix::operator * ( float scalar ) const
{
	GeoMatrix matrix;

	matrix._11 = _11 * scalar;
	matrix._12 = _12 * scalar;
	matrix._13 = _13 * scalar;
	matrix._14 = _14 * scalar;

	matrix._21 = _21 * scalar;
	matrix._22 = _22 * scalar;
	matrix._23 = _23 * scalar;
	matrix._24 = _24 * scalar;

	matrix._31 = _31 * scalar;
	matrix._32 = _32 * scalar;
	matrix._33 = _33 * scalar;
	matrix._34 = _34 * scalar;

	matrix._41 = _41 * scalar;
	matrix._42 = _42 * scalar;
	matrix._43 = _43 * scalar;
	matrix._44 = _44 * scalar;

	return matrix;
}

GeoMatrix GeoMatrix::operator / ( float scalar ) const
{
	GeoMatrix matrix;

	matrix._11 = _11 / scalar;
	matrix._12 = _12 / scalar;
	matrix._13 = _13 / scalar;
	matrix._14 = _14 / scalar;

	matrix._21 = _21 / scalar;
	matrix._22 = _22 / scalar;
	matrix._23 = _23 / scalar;
	matrix._24 = _24 / scalar;

	matrix._31 = _31 / scalar;
	matrix._32 = _32 / scalar;
	matrix._33 = _33 / scalar;
	matrix._34 = _34 / scalar;

	matrix._41 = _41 / scalar;
	matrix._42 = _42 / scalar;
	matrix._43 = _43 / scalar;
	matrix._44 = _44 / scalar;

	return matrix;
}

//friend OGLXMATRIX OGLXMATRIX::operator * ( float, const OGLXMATRIX& );
bool GeoMatrix::operator == ( const GeoMatrix& matrix ) const
{
	return  _11 == matrix._11 && _12 == matrix._12 && _13 == matrix._13 && _14 == matrix._14 &&
			_21 == matrix._21 && _22 == matrix._22 && _23 == matrix._23 && _24 == matrix._24 &&
			_31 == matrix._31 && _32 == matrix._32 && _33 == matrix._33 && _34 == matrix._34 &&
			_41 == matrix._41 && _42 == matrix._42 && _43 == matrix._43 && _44 == matrix._44;	
}

bool GeoMatrix::operator != ( const GeoMatrix& matrix ) const
{
	return	_11 != matrix._11 || _12 != matrix._12 || _13 != matrix._13 || _14 != matrix._14 ||
			_21 != matrix._21 || _22 != matrix._22 || _23 != matrix._23 || _24 != matrix._24 ||
			_31 != matrix._31 || _32 != matrix._32 || _33 != matrix._33 || _34 != matrix._34 ||
			_41 != matrix._41 || _42 != matrix._42 || _43 != matrix._43 || _44 != matrix._44;
}

#ifdef _WIN32
XMMATRIX GeoMatrix::ToXMMATRIX()
{
	XMFLOAT4X4 m = ToXMFLOAT4X4();
	return XMLoadFloat4x4( &m );
}

XMFLOAT4X4 GeoMatrix::ToXMFLOAT4X4()
{
	return 	*((XMFLOAT4X4*) this);
}
#endif

GeoMatrix GeoMatrix::Identity()
{
	GeoMatrix identity_matrix;
	identity_matrix._12 = identity_matrix._13 = identity_matrix._14 = 0;
	identity_matrix._21 = identity_matrix._23 = identity_matrix._24 = 0;
	identity_matrix._31 = identity_matrix._32 = identity_matrix._34 = 0;
	identity_matrix._41 = identity_matrix._42 = identity_matrix._43 = 0;
	identity_matrix._11 = identity_matrix._22 = identity_matrix._33 = identity_matrix._44 = 1;
	return identity_matrix;
}

GeoMatrix GeoMatrix::Inverse( float* pOutDeterminant )
{
	GeoMatrix inverted_matrix;

	float* mat = (float*) this;
	float* dst = (float*) &inverted_matrix;
	float tmp[12]; /* temp array for pairs */
	float src[16]; /* array of transpose source matrix */
	float det; /* determinant */

	/* transpose matrix */
	for (int i = 0; i < 4; i++) 
	{
		src[i] = mat[i*4];
		src[i + 4] = mat[i*4 + 1];
		src[i + 8] = mat[i*4 + 2];
		src[i + 12] = mat[i*4 + 3];
	}

	/* calculate pairs for first 8 elements (cofactors) */
	tmp[0] = src[10] * src[15];
	tmp[1] = src[11] * src[14];
	tmp[2] = src[9] * src[15];
	tmp[3] = src[11] * src[13];
	tmp[4] = src[9] * src[14];
	tmp[5] = src[10] * src[13];
	tmp[6] = src[8] * src[15];
	tmp[7] = src[11] * src[12];
	tmp[8] = src[8] * src[14];
	tmp[9] = src[10] * src[12];
	tmp[10] = src[8] * src[13];
	tmp[11] = src[9] * src[12];
	
	/* calculate first 8 elements (cofactors) */
	dst[0] = tmp[0]*src[5] + tmp[3]*src[6] + tmp[4]*src[7];
	dst[0] -= tmp[1]*src[5] + tmp[2]*src[6] + tmp[5]*src[7];
	dst[1] = tmp[1]*src[4] + tmp[6]*src[6] + tmp[9]*src[7];
	dst[1] -= tmp[0]*src[4] + tmp[7]*src[6] + tmp[8]*src[7];
	dst[2] = tmp[2]*src[4] + tmp[7]*src[5] + tmp[10]*src[7];
	dst[2] -= tmp[3]*src[4] + tmp[6]*src[5] + tmp[11]*src[7];
	dst[3] = tmp[5]*src[4] + tmp[8]*src[5] + tmp[11]*src[6];
	dst[3] -= tmp[4]*src[4] + tmp[9]*src[5] + tmp[10]*src[6];
	dst[4] = tmp[1]*src[1] + tmp[2]*src[2] + tmp[5]*src[3];
	dst[4] -= tmp[0]*src[1] + tmp[3]*src[2] + tmp[4]*src[3];
	dst[5] = tmp[0]*src[0] + tmp[7]*src[2] + tmp[8]*src[3];
	dst[5] -= tmp[1]*src[0] + tmp[6]*src[2] + tmp[9]*src[3];
	dst[6] = tmp[3]*src[0] + tmp[6]*src[1] + tmp[11]*src[3];
	dst[6] -= tmp[2]*src[0] + tmp[7]*src[1] + tmp[10]*src[3];
	dst[7] = tmp[4]*src[0] + tmp[9]*src[1] + tmp[10]*src[2];
	dst[7] -= tmp[5]*src[0] + tmp[8]*src[1] + tmp[11]*src[2];

	/* calculate pairs for second 8 elements (cofactors) */
	tmp[0] = src[2]*src[7];
	tmp[1] = src[3]*src[6];
	tmp[2] = src[1]*src[7];
	tmp[3] = src[3]*src[5];
	tmp[4] = src[1]*src[6];
	tmp[5] = src[2]*src[5];
	tmp[6] = src[0]*src[7];
	tmp[7] = src[3]*src[4];
	tmp[8] = src[0]*src[6];
	tmp[9] = src[2]*src[4];
	tmp[10] = src[0]*src[5];
	tmp[11] = src[1]*src[4];

	/* calculate second 8 elements (cofactors) */
	dst[8] = tmp[0]*src[13] + tmp[3]*src[14] + tmp[4]*src[15];
	dst[8] -= tmp[1]*src[13] + tmp[2]*src[14] + tmp[5]*src[15];
	dst[9] = tmp[1]*src[12] + tmp[6]*src[14] + tmp[9]*src[15];
	dst[9] -= tmp[0]*src[12] + tmp[7]*src[14] + tmp[8]*src[15];
	dst[10] = tmp[2]*src[12] + tmp[7]*src[13] + tmp[10]*src[15];
	dst[10]-= tmp[3]*src[12] + tmp[6]*src[13] + tmp[11]*src[15];
	dst[11] = tmp[5]*src[12] + tmp[8]*src[13] + tmp[11]*src[14];
	dst[11]-= tmp[4]*src[12] + tmp[9]*src[13] + tmp[10]*src[14];
	dst[12] = tmp[2]*src[10] + tmp[5]*src[11] + tmp[1]*src[9];
	dst[12]-= tmp[4]*src[11] + tmp[0]*src[9] + tmp[3]*src[10];
	dst[13] = tmp[8]*src[11] + tmp[0]*src[8] + tmp[7]*src[10];
	dst[13]-= tmp[6]*src[10] + tmp[9]*src[11] + tmp[1]*src[8];
	dst[14] = tmp[6]*src[9] + tmp[11]*src[11] + tmp[3]*src[8];
	dst[14]-= tmp[10]*src[11] + tmp[2]*src[8] + tmp[7]*src[9];
	dst[15] = tmp[10]*src[10] + tmp[4]*src[8] + tmp[9]*src[9];
	dst[15]-= tmp[8]*src[9] + tmp[11]*src[10] + tmp[5]*src[8];

	/* calculate determinant */
	det=src[0]*dst[0]+src[1]*dst[1]+src[2]*dst[2]+src[3]*dst[3];
	if( pOutDeterminant )
		*pOutDeterminant = det;

	/* calculate matrix inverse */
	det = 1/det;
	for (int j = 0; j < 16; j++)
		dst[j] *= det;

	return inverted_matrix;
}

GeoMatrix GeoMatrix::Transpose()
{
	GeoMatrix transposed_matrix;
	transposed_matrix._11 = _11;
	transposed_matrix._12 = _21;
	transposed_matrix._13 = _31;
	transposed_matrix._14 = _41;
	transposed_matrix._21 = _12;
	transposed_matrix._22 = _22;
	transposed_matrix._23 = _32;
	transposed_matrix._24 = _42;
	transposed_matrix._31 = _13;
	transposed_matrix._32 = _23;
	transposed_matrix._33 = _33;
	transposed_matrix._34 = _43;
	transposed_matrix._41 = _14;
	transposed_matrix._42 = _24;
	transposed_matrix._43 = _34;
	transposed_matrix._44 = _44;
	return transposed_matrix;
}

	/* PROJECTION MATRIX CONSTRUCTORS */
GeoMatrix GeoMatrix::MatrixPerspectiveFovRH( const float fovy, const float Aspect, const float zn, const float zf )
{
	const float yScale = 1.0f / tan( 0.5f*fovy );
	const float xScale = yScale / Aspect;

	GeoMatrix matrix;
	matrix._11 = xScale;
	matrix._22 = yScale;
	matrix._33 = zf/(zn - zf);
	matrix._34 = -1;
	matrix._43 = zn*zf/(zn - zf);

	//zero out all other entries
	matrix._21 = matrix._31 = matrix._41 = 0;
	matrix._12 = matrix._32 = matrix._42 = 0;
	matrix._13 = matrix._23 = 0;
	matrix._14 = matrix._24 = matrix._44 = 0;

	return matrix;
}

GeoMatrix GeoMatrix::MatrixPerspectiveFovLH( const float fovy, const float Aspect, const float zn, const float zf )
{
	const float yScale = 1.0f / tan( 0.5f*fovy );
	const float xScale = yScale / Aspect;

	GeoMatrix matrix;
	matrix._11 = xScale;
	matrix._22 = yScale;
	matrix._33 = zf/(zn - zf);
	matrix._34 = 1;
	matrix._43 = -zn*zf/(zn - zf);

	//zero out all other entries
	matrix._21 = matrix._31 = matrix._41 = 0;
	matrix._12 = matrix._32 = matrix._42 = 0;
	matrix._13 = matrix._23 = 0;
	matrix._14 = matrix._24 = matrix._44 = 0;

	return matrix;
}

	/* VIEW MATRIX CONSTRUCTORS */
GeoMatrix GeoMatrix::LookAtRH( const GeoVector& eye_position, const GeoVector& look_at_position, const GeoVector& up_direction )
{
	GeoVector zaxis = (eye_position - look_at_position).Normalize();
	GeoVector xaxis = GeoVector(up_direction).Cross( zaxis ).Normalize();
	GeoVector yaxis = zaxis.Cross( xaxis );

	GeoMatrix matrix;

	//set column one
	matrix._11 = xaxis.x;
	matrix._21 = xaxis.y;
	matrix._31 = xaxis.z;
	matrix._41 = -xaxis.Dot( eye_position ); 

	// set column two
	matrix._12 = yaxis.x;
	matrix._22 = yaxis.y;
	matrix._32 = yaxis.z;
	matrix._42 = -yaxis.Dot( eye_position );

	// set column three
	matrix._13 = zaxis.x;
	matrix._23 = zaxis.y;
	matrix._33 = zaxis.z;
	matrix._43 = -zaxis.Dot( eye_position );

	// set column four
	matrix._14 = matrix._24 = matrix._34 = 0;
	matrix._44 = 1;

	return matrix;
}

GeoMatrix GeoMatrix::LookAtLH( const GeoVector& eye_position, const GeoVector& look_at_position, const GeoVector& up_direction )
{
	GeoVector zaxis = (look_at_position - eye_position).Normalize();
	GeoVector xaxis = GeoVector(up_direction).Cross( zaxis ).Normalize();
	GeoVector yaxis = zaxis.Cross( xaxis );

	GeoMatrix matrix;
	//set column one
	matrix._11 = xaxis.x;
	matrix._21 = xaxis.y;
	matrix._31 = xaxis.z;
	matrix._41 = -xaxis.Dot( eye_position );
	
	//set column two
	matrix._12 = yaxis.x;
	matrix._22 = yaxis.y;
	matrix._32 = yaxis.z;
	matrix._42 = -yaxis.Dot( eye_position ); 

	// set column three
	matrix._13 = zaxis.x;
	matrix._23 = zaxis.y;
	matrix._33 = zaxis.z;
	matrix._43 = -zaxis.Dot( eye_position ); 

	// set column four
	matrix._14 = matrix._24 = matrix._34 = 0;
	matrix._44 = 1;

	return matrix;
}

GeoMatrix GeoMatrix::Reflection( GeoVector reflection_plane )
{
	GeoMatrix reflection_matrix;

	float plane_magnitude = GeoVector( reflection_plane.x, reflection_plane.y, reflection_plane.z, 0 ).Length();
	const GeoVector normalized_plane = reflection_plane / plane_magnitude;

	//row one
	reflection_matrix._11 = -2 * normalized_plane.x * normalized_plane.x + 1;
	reflection_matrix._12 = -2 * normalized_plane.y * normalized_plane.x;
	reflection_matrix._13 = -2 * normalized_plane.z * normalized_plane.x;
	reflection_matrix._14 = 0;

	//row two
	reflection_matrix._21 = -2 * normalized_plane.x * normalized_plane.y;
	reflection_matrix._22 = -2 * normalized_plane.y * normalized_plane.y + 1;
	reflection_matrix._23 = -2 * normalized_plane.z * normalized_plane.y;
	reflection_matrix._24 = 0;

	//row three
	reflection_matrix._31 = -2 * normalized_plane.x * normalized_plane.z;
	reflection_matrix._32 = -2 * normalized_plane.y * normalized_plane.z;
	reflection_matrix._33 = -2 * normalized_plane.z * normalized_plane.z + 1;
	reflection_matrix._34 = 0;

	//row four
	reflection_matrix._41 = -2 * normalized_plane.x * normalized_plane.w;
	reflection_matrix._42 = -2 * normalized_plane.y * normalized_plane.w;
	reflection_matrix._43 = -2 * normalized_plane.z * normalized_plane.w;
	reflection_matrix._44 = 1;

	return reflection_matrix;
}

GeoMatrix GeoMatrix::Translation( const float x, const float y, const float z )
{
	GeoMatrix translation_matrix;
	translation_matrix._12 = translation_matrix._13 = translation_matrix._14 = 0;
	translation_matrix._21 = translation_matrix._23 = translation_matrix._24 = 0;
	translation_matrix._31 = translation_matrix._32 = translation_matrix._34 = 0;
	translation_matrix._11 = translation_matrix._22 = translation_matrix._33 = translation_matrix._44 = 1;

	//row four
	translation_matrix._41 = x;
	translation_matrix._42 = y;
	translation_matrix._43 = z;

	return translation_matrix;
}

GeoMatrix GeoMatrix::Scaling( const float x, const float y, const float z )
{
	GeoMatrix scaling_matrix;
	scaling_matrix._12 = scaling_matrix._13 = scaling_matrix._14 = 0;
	scaling_matrix._21 = scaling_matrix._23 = scaling_matrix._24 = 0;
	scaling_matrix._31 = scaling_matrix._32 = scaling_matrix._34 = 0;
	scaling_matrix._41 = scaling_matrix._42 = scaling_matrix._43 = 0;
	scaling_matrix._11 = x;
	scaling_matrix._22 = y;
	scaling_matrix._33 = z;
	scaling_matrix._44 = 1;	
	return scaling_matrix;
}

GeoMatrix GeoMatrix::Scaling( const float scale )
{
	return GeoMatrix::Scaling( scale, scale, scale );
}

GeoMatrix GeoMatrix::RotationYRH( const float angle )
{
	const float sine = sin( angle );
	const float cosine = cos( angle );
	GeoMatrix rotation_matrix;
	rotation_matrix._11 = cosine; rotation_matrix._12 = 0; rotation_matrix._13 = sine; rotation_matrix._14 = 0;
	rotation_matrix._21 = rotation_matrix._23 = rotation_matrix._24 = 0; rotation_matrix._22 = 1;
	rotation_matrix._31 = -sine; rotation_matrix._32 = 0; rotation_matrix._33 = cosine; rotation_matrix._34 = 0;
	rotation_matrix._41 = rotation_matrix._42 = rotation_matrix._43 = 0; rotation_matrix._44 = 1;
	return rotation_matrix;
}