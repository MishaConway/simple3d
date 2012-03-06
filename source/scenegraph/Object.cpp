#include "Object.h"

GraphicsDevice* Object::pGraphicsDevice;

void Object::BindGraphicsDevice( GraphicsDevice* pGraphicsDevice )
{
	Object::pGraphicsDevice = pGraphicsDevice;
}

Object::Object()
{
	SetIdentity();
	rotating = false;
	degrees_per_second = objectspace_angular_deceleration = 0;
	rotated_in_objectspace = false;
}

bool Object::Update( const float elapsed_seconds )
{
    if( rotating )
    {
		RotateInObjectspace( objectspace_rotation_axis, elapsed_seconds * degrees_per_second );
        const float old_degrees_per_second = degrees_per_second;
        degrees_per_second -= (float) copysign( objectspace_angular_deceleration * elapsed_seconds, degrees_per_second );
        rotating = (int)copysign( 1, old_degrees_per_second ) == (int)copysign( 1, degrees_per_second ); 
    }
	return true;
}

bool Object::IsRenderable()
{
	return false;
}

void Object::SetIdentity()
{
	world_transform = GeoMatrix::Identity();
}

void Object::Translate( const float x, const float y, const float z )
{
	world_transform *= GeoMatrix::Translation( x, y, z );
}

void Object::Translate( const GeoVector& translation )
{
	Translate( translation.x, translation.y, translation.z );
}

void Object::Scale( const float x, const float y, const float z )
{
	world_transform *= GeoMatrix::Scaling( x, y, z );
}

void Object::RotateY( const float angle_in_degrees )
{
	world_transform *= GeoMatrix::RotationYRH( GeoConvertToRadians(angle_in_degrees) );
	
	//XMStoreFloat4x4( &world_transform, XMMatrixMultiply( XMLoadFloat4x4(&world_transform), XMMatrixRotationY( XMConvertToRadians(angle_in_degrees) ) ));		
}


void Object::RotateInWorldspace( const GeoVector& rotation_axis, const float degrees )
{
    world_transform *= GeoQuaternion( rotation_axis, degrees ).ToMatrix();
}

void Object::SetObjectspaceRotation( const GeoVector& rotation_axis, const float degrees )
{
    objectspace_rotation_axis = rotation_axis;
    rotated_in_objectspace = true;
    objectspace_quaternion =  GeoQuaternion( rotation_axis, degrees ); 
}


void Object::RotateInObjectspace( const GeoVector& rotation_axis, const float degrees )
{
    objectspace_rotation_axis = rotation_axis;

	if( !rotated_in_objectspace )
	{
		rotated_in_objectspace = true;
		objectspace_quaternion = GeoQuaternion( rotation_axis, 0 ); 
	}

    objectspace_quaternion =  GeoQuaternion( objectspace_rotation_axis, degrees  ) * objectspace_quaternion;
}

void Object::SetRotationalVelocity( const GeoVector& rotation_axis, const float degrees_per_second, const float objectspace_angular_deceleration  )
{
	objectspace_rotation_axis = rotation_axis;
	this->degrees_per_second = degrees_per_second;
    this->objectspace_angular_deceleration = objectspace_angular_deceleration;
	RotateInObjectspace( rotation_axis, 0 );
	rotating = true;
}

float Object::GetObjectspaceAngle()
{
    return objectspace_quaternion.GetAngle();
}

GeoVector Object::GetObjectspaceAxis()
{
    return objectspace_quaternion.GetAxis();
}

GeoMatrix Object::GetWorldTransform()
{
	GeoMatrix world_transform_after_objectspace_rotations;
	if( rotating || rotated_in_objectspace )
	{		
		world_transform_after_objectspace_rotations = objectspace_quaternion.ToMatrix() * world_transform;
	}
	else
		world_transform_after_objectspace_rotations = world_transform;
	return world_transform_after_objectspace_rotations;
}

GeoMatrix Object::GetWorldInverseTranspose()
{
	return GetWorldTransform().Inverse(0).Transpose();
	//return XMMatrixTranspose(XMMatrixInverse( 0, XMLoadFloat4x4(&wtransform) ) );
}

