#include "Camera.h"

Camera::Camera(){}
Camera::Camera( const unsigned int width, const unsigned int height, const float fovy, const float near_z, const float far_z, const GeoVector& eye_position, const GeoVector& focus_position )
{
	this->width = width;
	this->height = height;
	this->fovy = fovy;
	this->near_z = near_z;
	this->far_z = far_z;
	projection_transform = GeoMatrix::MatrixPerspectiveFovRH( XMConvertToRadians(fovy),(float)width / (float)height, near_z, far_z );

	current_eye_position = eye_position;
	current_focus_position = focus_position;
	camera_speed = 1;
	camera_is_moving = false;
	view_transform = GeoMatrix::LookAtRH( current_eye_position, current_focus_position, GeoVector( 0, 1, 0 ) );
}

void Camera::SetCameraSpeed( const float camera_speed )
{
	this->camera_speed = camera_speed;
}

void Camera::SetTargetView( const GeoVector& target_eye_position, const GeoVector& target_focus_position )
{
	start_eye_position = current_eye_position;
	start_focus_position = current_focus_position;
	
	this->target_eye_position = target_eye_position;
	this->target_focus_position = target_focus_position;
	camera_is_moving = true;

	eye_to_target = target_eye_position - current_eye_position;
	focus_to_target = target_focus_position - current_focus_position;

	eye_position_distance_to_move = eye_to_target.Length();
	focus_position_distance_to_move = focus_to_target.Length();

	interpolation_factor = 0; 	
}

void Camera::SetTargetView( const GeoVector& target_eye_position, const GeoVector& target_direction, const float distance_from_target  )
{
	start_eye_position = current_eye_position;
	start_focus_position = current_focus_position;
	
	this->target_eye_position = target_eye_position - target_direction * distance_from_target;
	target_focus_position = target_eye_position + target_direction;
	camera_is_moving = true;

	eye_to_target =  target_eye_position - current_eye_position;
	focus_to_target =  target_focus_position - current_focus_position;

	eye_position_distance_to_move = eye_to_target.Length();
	focus_position_distance_to_move = focus_to_target.Length();

	interpolation_factor = 0; 	
}

void Camera::Translate( const GeoVector& translation )
{
	current_eye_position += translation;
	current_focus_position += translation;
	view_transform = GeoMatrix::LookAtRH( current_eye_position, current_focus_position, GeoVector( 0, 1, 0 ) );
}

bool Camera::IsMoving()
{
	return camera_is_moving;
}

void Camera::Update( const float elapsed_seconds )
{
	if( camera_is_moving )
	{
		interpolation_factor +=  camera_speed * elapsed_seconds / eye_position_distance_to_move; 
		if( interpolation_factor >= 1.0f )
		{
			camera_is_moving = false;
			current_eye_position = target_eye_position;
			current_focus_position = target_focus_position;
		}
		else
		{
			current_eye_position = start_eye_position + eye_to_target * interpolation_factor;
			current_focus_position = start_focus_position + focus_to_target * interpolation_factor;
		}

		view_transform = GeoMatrix::LookAtRH( current_eye_position, current_focus_position, GeoVector( 0, 1, 0 ) );
	}
}

unsigned int Camera::GetWidth()
{
	return width;
}

unsigned int Camera::GetHeight()
{
	return height;
}

GeoMatrix Camera::GetProjectionTransform()
{
	return projection_transform;	
}

GeoMatrix Camera::GetViewTransform()
{
	return view_transform;
}

GeoMatrix Camera::GetReflectedViewTransform( const GeoVector& reflection_plane )
{
	return GeoMatrix::Reflection( reflection_plane ) * GetViewTransform();
}

GeoVector Camera::GetEyePosition()
{
	return current_eye_position;
}

GeoVector Camera::GetFocusPosition()
{
	return current_focus_position;
}

GeoVector Camera::GetEyeDirection()
{
	return GetFocusPosition() - GetEyePosition(); 
}

GeoVector Camera::GetEyeDirectionNormalized()
{
	return GetEyeDirection().Normalize();
}

GeoVector Camera::ProjectIntoScreenspace( const GeoVector& worldspace_vertex )
{
    const float    HalfViewportWidth = width * 0.5f;  
    const float    HalfViewportHeight = height * 0.5f;  
    GeoVector Scale(HalfViewportWidth,   -HalfViewportHeight,  1,  0.0f);  
    GeoVector Offset(HalfViewportWidth, HalfViewportHeight, 0, 0.0f);  
	return ( view_transform * projection_transform ).TransformCoord( GeoVector(worldspace_vertex, 1 )) * Scale + Offset;
}