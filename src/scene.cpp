#include "scene.hpp"
#include "terrain.hpp"

/** This file contains the custom implementation associated the scene 
*  The (*) indicates elements that are specific to the use of the camera mode and that should be handled if you want to use this effect in another scene. */

using namespace cgp;


void scene_structure::update_mouse(float xpos, float ypos)
{
	
}


// The main function implementing the Flying Mode
void scene_structure::update_camera(float xpos, float ypos)
{
	inputs_keyboard_parameters const& keyboard = inputs.keyboard;
	camera_head& camera = environment.camera;

	// The camera moves forward all the time
	//   We consider in this example a constant velocity, so the displacement is: velocity * dt * front-camera-vector
	float const dt = timer.update();

	vec2 new_pos = vec2{ xpos, ypos };
	mouse_speed = (new_pos - mouse_pos) / dt;
	mouse_pos = new_pos;

	// The camera rotates if we press on the arrow keys
	//  The rotation is only applied to the roll and pitch degrees of freedom.
	float const pitch = 0.00001f; // speed of the pitch
	float const roll  = 0.00001f; // speed of the roll
	float const yaw = 0.00001f; // speed of the yaw

	float pitch_angle = - pitch * mouse_speed.y;
	float yaw_angle = - yaw * mouse_speed.x;

	if ( !(camera.front().z < -0.99 && pitch_angle < 0) && !(camera.front().z > 0.99 && pitch_angle > 0) )
		camera.manipulator_rotate_roll_pitch_yaw(0, pitch_angle, 0);
	quaternion rot = camera.orientation_camera.data;
	camera.orientation_camera = rotation_transform::convert_axis_angle_to_quaternion({ 0,0,1 }, yaw_angle) * rot;

	speed = vec3{0,0,0};
	if (keyboard.up)
	{
		vec3 front = vec3{ camera.front().x, camera.front().y, 0 };
		front /= (front.x * front.x + front.y * front.y);
		speed = gui.speed * front;
		camera.position_camera += speed * dt;
	}
	if (keyboard.down)
	{
		vec3 front = vec3{ camera.front().x, camera.front().y, 0 };
		front /= (front.x * front.x + front.y * front.y);
		speed = -gui.speed * front;
		camera.position_camera += speed * dt;
	}
	if (keyboard.right)
	{
		speed = gui.speed * camera.right();
		camera.position_camera += speed * dt;
	}
	if (keyboard.left)
	{
		speed = -gui.speed * camera.right();
		camera.position_camera += speed * dt;
	}

	pos = camera.position();
}

void scene_structure::initialize()
{
	// Default frame
	global_frame.initialize(mesh_primitive_frame(), "Frame");

	float c = 10;

	// Load the terrain (display a debug message as the loading can take some time)
	std::cout << " \nLoad terrain file ..." << std::endl;
	//mesh terrain_mesh = mesh_primitive_quadrangle({ -10, -10, 0 }, { 10, -10, 0 }, { 10, 10, 0 }, { -10, 10, 0 });
	mesh terrain_mesh = create_terrain_mesh(100, 100);
	terrain.initialize(terrain_mesh, "Terrain");
	demilune.initialize(mesh_load_file_obj("assets/demilune.obj"), "Demi-lune");
	std::cout << " [OK] Terrain loaded\n" << std::endl;

	// Load the texture of the terrain
	std::cout << " \nLoad terrain texture ..." << std::endl;
	//terrain.texture = opengl_load_texture_image("assets/mountain.jpg");
	std::cout << " [OK] Texture loaded\n" << std::endl;
	
	// Initial placement of the camera
	environment.camera.position_camera = { 0.0f, 0.0f, 2.0f };
	environment.camera.manipulator_rotate_roll_pitch_yaw(0, Pi / 2.0f, 0);

	demilune.transform.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, Pi/2.0f);
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
	draw(demilune, environment);
}



void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::SliderFloat("Speed", &gui.speed, 0.2f, 5.0f); // Set the camera velocity
}


