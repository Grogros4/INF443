#include "environment_camera_head.hpp"

using namespace cgp;

scene_environment_camera_head::scene_environment_camera_head()
{
	background_color = { 1,1,1 };
	projection = camera_projection::perspective(50.0f * Pi / 180, 1.0f, 0.1f, 500.0f);
}

void opengl_uniform(GLuint shader, scene_environment_camera_head const& environment)
{
	// Basic uniform parameters
	opengl_uniform(shader, "projection", environment.projection.matrix());
	opengl_uniform(shader, "view", environment.camera.matrix_view());
	opengl_uniform(shader, "light", environment.light);
}