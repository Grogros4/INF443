#include "scene.hpp"

/** This file contains the custom implementation associated the scene 
*  The (*) indicates elements that are specific to the use of the camera mode and that should be handled if you want to use this effect in another scene. */

using namespace cgp;


// The main function implementing the Flying Mode
void scene_structure::update_camera()
{
	inputs_keyboard_parameters const& keyboard = inputs.keyboard;
	camera_head& camera = environment.camera;

	// The camera moves forward all the time
	//   We consider in this example a constant velocity, so the displacement is: velocity * dt * front-camera-vector
	float const dt = timer.update();
	vec3 const forward_displacement = gui.speed * dt * camera.front();

	// The camera rotates if we press on the arrow keys
	//  The rotation is only applied to the roll and pitch degrees of freedom.
	float const pitch = 0.5f; // speed of the pitch
	float const roll  = 0.7f; // speed of the roll
	float const yaw = 0.7f; // speed of the yaw
	if (keyboard.up)
		camera.position_camera += forward_displacement; 
	if (keyboard.down)
		camera.position_camera -= forward_displacement;
	if (keyboard.right)
		camera.manipulator_rotate_roll_pitch_yaw( 0, 0, -yaw * dt);
	if (keyboard.left)
		camera.manipulator_rotate_roll_pitch_yaw( 0, 0, yaw * dt);

	pos = camera.position();
	speed = gui.speed * camera.front();
}

void scene_structure::initialize()
{
	// Default frame
	global_frame.initialize(mesh_primitive_frame(), "Frame");

	float c = 10;

	// Load the terrain (display a debug message as the loading can take some time)
	std::cout << " \nLoad terrain file ..." << std::endl;
	mesh terrain_mesh = mesh_primitive_quadrangle({ -10, -10, -2 }, { 10, -10, -2 }, { 10, 10, -2 }, { -10, 10, -2 });
	terrain.initialize(terrain_mesh, "Terrain");
	std::cout << " [OK] Terrain loaded\n" << std::endl;

	// Load the texture of the terrain
	std::cout << " \nLoad terrain texture ..." << std::endl;
	//terrain.texture = opengl_load_texture_image("assets/mountain.jpg");
	std::cout << " [OK] Texture loaded\n" << std::endl;
	
	// Initial placement of the camera
	environment.camera.position_camera = { 0.0f, 0.0f, 0.0f };
	environment.camera.manipulator_rotate_roll_pitch_yaw(0, Pi / 2.0f, 0);
}



void scene_structure::display()
{	
	// set the light position to the camera
	environment.light = environment.camera.position(); 
	environment.speed = speed;
	environment.c = c;

	// The standard frame
	if (gui.display_frame)
		draw(global_frame, environment);
	
	// Displaying the shape deformed by the shader
	draw(terrain, environment);
}



void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::SliderFloat("Speed", &gui.speed, 0.2f, 5.0f); // Set the camera velocity
}


