#pragma once

#include "../geo/GeoMatrix.h"
#include "../geo/GeoQuaternion.h"

class Camera
{
public:
	Camera();
	Camera( const unsigned int width, const unsigned int height, const float fovy, const float near_z, const float far_z, const GeoVector& default_eye_position, const GeoVector& default_focus_position );
	void Translate( const GeoVector& translation );
	void SetCameraSpeed( const float camera_speed );
	void SetTargetView( const GeoVector& target_eye_position, const GeoVector& target_direction, const float distance_from_target );
	void SetTargetView( const GeoVector& target_eye_position, const GeoVector& target_focus_position );
	void Update( const float elapsed_seconds );
	bool IsMoving();
	unsigned int GetWidth();
	unsigned int GetHeight();
    void SetWidthHeight( const unsigned int width, const unsigned int height );
	GeoMatrix GetProjectionTransform();
	GeoMatrix GetViewTransform();
	GeoMatrix GetReflectedViewTransform( const GeoVector& reflection_plane );
	GeoVector GetEyePosition();
	GeoVector GetFocusPosition();
	GeoVector GetEyeDirection();
	GeoVector GetEyeDirectionNormalized();

	GeoFloat2 ScreenspaceToClipspace( const GeoFloat2& screensapce_vertex );

	GeoVector ProjectIntoScreenspace( const GeoVector& worldspace_vertex );
private:
	GeoMatrix projection_transform;
	unsigned int width;
	unsigned int height; 
	float fovy;
	float near_z; 
	float far_z;

	GeoMatrix view_transform;
	
	GeoVector target_eye_position, target_focus_position;
	GeoVector start_eye_position, start_focus_position;
	GeoVector current_eye_position, current_focus_position;
	GeoVector eye_to_target, focus_to_target;
	float eye_position_distance_to_move, focus_position_distance_to_move;
	float interpolation_factor;

	float camera_speed;
	bool camera_is_moving;
};