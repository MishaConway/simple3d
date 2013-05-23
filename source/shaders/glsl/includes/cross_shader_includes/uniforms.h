uniform mat4 WorldTransform;
uniform mat4 ViewTransform;
uniform mat4 ProjectionTransform;
uniform mat4 WorldInverseTranspose;

uniform float viewport_width;
uniform float viewport_height;

uniform vec4 color;
uniform vec4 specular_color;
uniform float specular_power;

uniform vec3 eye_position;
uniform vec4 light_source;

uniform int compute_projective_texture_coordinates;

uniform vec4 clip_plane;
uniform int clipping_enabled;

