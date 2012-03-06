#pragma once

#include "../graphics/inc.h"
#include "../geo/GeoQuaternion.h"

class Object
{
public:
	Object();
	virtual bool IsRenderable();
	virtual bool Update( const float elapsed_seconds );
	void SetIdentity();
	void Translate( const float x, const float y, const float z );
	void Translate( const GeoVector& translation );
	void Scale( const float x, const float y, const float z );
	void RotateY( const float angle_in_degrees );
	
    
    void SetObjectspaceRotation( const GeoVector& rotation_axis, const float degrees );
    void RotateInObjectspace( const GeoVector& rotation_axis, const float degrees );
    float GetObjectspaceAngle();
    GeoVector GetObjectspaceAxis();

	

    void RotateInWorldspace( const GeoVector& rotation_axis, const float degrees );
    
    

	void SetRevolutionVelocity( const GeoVector& revolve_axis, const float radius, const float degrees_per_second );
	void SetRotationalVelocity( const GeoVector& rotation_axis, const float degrees_per_second, const float objectspace_angular_deceleration = 0 );

	
    void SetUserData( const std::string& key, const std::string& value );
    std::string GetUserData( const std::string& key );

	GeoMatrix GetWorldTransform();
	GeoMatrix   GetWorldInverseTranspose();

	static void BindGraphicsDevice( GraphicsDevice* pGraphicsDevice );
protected:
	GeoMatrix world_transform;
	GeoVector objectspace_rotation_axis;
	GeoQuaternion objectspace_quaternion;
	float degrees_per_second, objectspace_angular_deceleration;
	bool rotating;
	bool rotated_in_objectspace;
	bool revolving;
	float revolve_degrees_per_second;
	GeoVector revolution_axis;
	float degrees_revolved;
    std::map<std::string, std::string> user_data;
	static GraphicsDevice* pGraphicsDevice;
};