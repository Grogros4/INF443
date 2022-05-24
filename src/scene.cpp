#include "scene.hpp"

/** This file contains the custom implementation associated the scene 
*  The (*) indicates elements that are specific to the use of the camera mode and that should be handled if you want to use this effect in another scene. */

using namespace cgp;

void scene_structure::initialize()
{
	// Default frame
	global_frame.initialize(mesh_primitive_frame(), "Frame");

	// Initializing terrain
	terrain.initialize(&environment, 100, chunk_size);

	// Initializing demilunes
	demi_lunes.initialize(&environment);

	// Initializing lamp grid
    l1.initialize(&environment, vec3{ -5, 0, terrain.evaluate_hills_height(-5,0) + 0.1}, "lamp1", 0.5f);
	l2.initialize(&environment, vec3{ 5, 0, terrain.evaluate_hills_height(5,0) + 0.1 }, "lamp2", 0.5f);

	// Initializing sky
	sky.initialize(&environment, 400, 1000, 0.5);

	// Initializing the car
	// Key 3D positions
	buffer<vec3> key_positions = { {-10,0,10}, {-10,0,10}, {10,0,10}, {10,0,10}};
	// Key times (time at which the position must pass in the corresponding position)
	buffer<float> key_times = { 0.0f, 1.0f, 2.0f, 3.0f};
	// car mesh
	mesh_drawable sphere;
	sphere.initialize(mesh_primitive_sphere(0.7f), "sphere");
	car1.initialize(&environment, sphere, key_positions, key_times);

	// Initial placement of the camera
	environment.camera.position_camera = { 0.0f, 0.0f, 2.0f };
	environment.camera.manipulator_rotate_roll_pitch_yaw(0, Pi / 2.0f, 0);

	//Initializing the ortho camera
	environment_hud.projection = camera_projection::orthographic(-2, 2, -1, 1, -1, 1);
	environment_hud.light = { 0,0,1 };
	environment_hud.light_speed = 30000000.0f;
	environment_hud.speed = { 0,0,0 };
	environment_hud.obj_speed = { 0,0,0 };


	// Initialize HUD texture
	quad.initialize(mesh_primitive_disc(0.1, { -1.6,-0.6,-0.01f }, { 0,0,1 }, 100), "Quad");
	quad.texture = opengl_load_texture_image("assets/clock/empty_clock.png");
	//quad.anisotropic_scale.y = 2;
	second.initialize(mesh_primitive_quadrangle({ -0.001,0,0 }, { -0.001,0.08,0 }, { 0.001,0.08,0 }, { 0.001,0,0 }), "Second");
	//second.anisotropic_scale.y = 2;
	second.transform.translation = { -1.6,-0.6, 0 };
	second.shading.color = { 1,0,0 };
}


void scene_structure::display()
{	
	std::cout << norm(speed) << std::endl;

	// Updating world parameters
	environment.light = vec3{0, 0, 50};
	environment.speed = speed;
	environment.light_speed = c;
	environment.obj_speed = { 0,0,0 };

	// Displaying terrain (and trees)
	terrain.display();

	// Displaying sky (and stars)
	sky.display();

	// Displaying demilunes
	demi_lunes.display();

	// Updating and displaying lamps
	l1.update(pos, speed, c);
	l2.update(pos, speed, c);

	// Updating and displaying clock HUD
	clock_timer.update(speed, c);
	draw(quad, environment_hud);
	second.transform.rotation = rotation_transform::from_axis_angle({0,0,1}, -(int(clock_timer.t) % 60) * Pi / 30);
	draw(second, environment_hud);

	// Updating and displaying car
	car1.update(pos, speed, c);
}