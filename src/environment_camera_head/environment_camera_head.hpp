
#include "cgp/cgp.hpp"

// Standard scene environment but replacing the common rotating camera with a camera_head
//  camera_head is a camera model that rotates around its own position - this appropriate to model free-flight.
struct scene_environment_camera_head {

	// Standard parameters (same as basic environment)
	cgp::vec3 background_color;
	cgp::camera_projection projection;
	cgp::vec3 light;  // stores the light position

	// Change the standard rotating camera to a "head camera" more appropriate to model free-flight (*)
	cgp::camera_head camera;

	scene_environment_camera_head();
};

struct scene_environment_player_head : public scene_environment_camera_head {
	cgp::vec3 speed;
	float c;
};


void opengl_uniform(GLuint shader, scene_environment_camera_head const& environment);
void opengl_uniform(GLuint shader, scene_environment_player_head const& environment);