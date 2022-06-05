#include "environment_camera_head.hpp"

using namespace cgp;


scene_environment_camera_head::scene_environment_camera_head()
{
	background_color = { 0,0,0.5 };
	projection = camera_projection::perspective(50.0f * Pi / 180, 1.0f, 0.1f, 500.0f);
}

void opengl_uniform(GLuint shader, scene_environment_camera_head const& environment)
{
	// Basic uniform parameters
	opengl_uniform(shader, "projection", environment.projection.matrix());
	opengl_uniform(shader, "view", environment.camera.matrix_view());
	opengl_uniform(shader, "light", environment.light);
	opengl_uniform(shader, "player_speed", environment.speed);
	opengl_uniform(shader, "c", environment.light_speed);
	opengl_uniform(shader, "obj_speed", environment.obj_speed);
	opengl_uniform(shader, "stellar_aberration", environment.stellar_aberration);
	opengl_uniform(shader, "doppler_effect", environment.doppler_effect);
	opengl_uniform(shader, "relativistic_brightness", environment.relativistic_brightness);
}