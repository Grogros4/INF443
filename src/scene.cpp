#include "scene.hpp"
#include "terrain.hpp"

/** This file contains the custom implementation associated the scene 
*  The (*) indicates elements that are specific to the use of the camera mode and that should be handled if you want to use this effect in another scene. */

using namespace cgp;


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

	vec3 acc = vec3{ 0.0f, 0.0f, 0.0f };

	vec3 front = vec3{ camera.front().x, camera.front().y, 0 };
	front /= sqrt(front.x * front.x + front.y * front.y);

	if (keyboard.up)
	{
		acc += front;
	}
	if (keyboard.down)
	{
		acc += - front;
	}
	if (keyboard.right)
	{
		acc += camera.right();
	}
	if (keyboard.left)
	{
		acc += - camera.right();
	}

float acc_norm = sqrt(acc.x * acc.x + acc.y * acc.y + acc.z * acc.z);
if (acc_norm > 0.01)
acc = (walk_acc / acc_norm) * acc;
speed += acc * dt - f * speed * dt;
//float speed_norm = sqrt(speed.x * speed.x + speed.y * speed.y + speed.z * speed.z);
//if (speed_norm > speed_max)
//	speed = (speed_max / speed_norm) * speed;
camera.position_camera += speed * dt;

pos = camera.position();
}

void scene_structure::initialize_demilune()
{
	mesh_drawable demilune_base;
	mesh_drawable demilune_saumon;
	mesh_drawable demilune_toit;
	mesh_drawable demilune_batiment;
	mesh_drawable demilune_briques;
	mesh_drawable demilune_blanc;
	mesh_drawable demilune_glass;
	mesh_drawable demilune_bandes;

	demilune_base.initialize(mesh_primitive_cube({ 0,-2, 0 }), "demilune_base");
	demilune_saumon.initialize(mesh_load_file_obj("assets/objects/demilune_saumon.obj"), "demilune_saumon");
	demilune_toit.initialize(mesh_load_file_obj("assets/objects/demilune_toit.obj"), "demilune_toit");
	demilune_batiment.initialize(mesh_load_file_obj("assets/objects/demilune_batiment.obj"), "demilune_batiment");
	demilune_briques.initialize(mesh_load_file_obj("assets/objects/demilune_briques.obj"), "demilune_briques");
	demilune_blanc.initialize(mesh_load_file_obj("assets/objects/demilune_blanc.obj"), "demilune_blanc");
	demilune_glass.initialize(mesh_load_file_obj("assets/objects/demilune_glass.obj"), "demilune_glass");
	demilune_bandes.initialize(mesh_load_file_obj("assets/objects/demilune_bandes.obj"), "demilune_bandes");

	demilune_saumon.shading.color = 2 * vec3{ 0.171441, 0.061246, 0.064803 };
	demilune_saumon.shading.phong = shading_parameters_phong::phong_parameters{1, 1, 0.5, 250};
	demilune_toit.shading.color = vec3{ 1, 1, 1 };
	demilune_toit.shading.phong = shading_parameters_phong::phong_parameters{ 1, 1, 0.5, 250 };
	demilune_batiment.shading.color = vec3{ 0.342383, 0.342383, 0.342383 };
	demilune_batiment.shading.phong = shading_parameters_phong::phong_parameters{ 1, 1, 0.5, 360 };
	demilune_briques.shading.color = vec3{ 0.076185, 0.035601, 0.039546 };
	demilune_briques.shading.phong = shading_parameters_phong::phong_parameters{ 1, 1, 0.5, 250 };
	demilune_blanc.shading.color = vec3{ 0.800000, 0.800000, 0.800000 };
	demilune_blanc.shading.phong = shading_parameters_phong::phong_parameters{ 1, 1, 0.5, 250 };
	demilune_glass.shading.color = vec3{ 0.200000, 0.200000, 0.200000 };
	demilune_glass.shading.phong = shading_parameters_phong::phong_parameters{ 1, 0, 0.5, 250 };
	demilune_bandes.shading.color = vec3{ 0.400000, 0.000000, 0.000000 };
	demilune_bandes.shading.phong = shading_parameters_phong::phong_parameters{ 1, 1, 0.5, 250 };

	demilune.add(demilune_base);
	demilune.add(demilune_saumon, "demilune_base");
	demilune.add(demilune_toit, "demilune_base");
	demilune.add(demilune_batiment, "demilune_base");
	demilune.add(demilune_briques, "demilune_base");
	demilune.add(demilune_blanc, "demilune_base");
	demilune.add(demilune_glass, "demilune_base");
	demilune.add(demilune_bandes, "demilune_base");

	demilune["demilune_base"].transform.rotation = rotation_transform::from_axis_angle({1,0,0}, Pi / 2.0f);
}

void scene_structure::initialize()
{
	c = 100000.0f;
	// Default frame
	global_frame.initialize(mesh_primitive_frame(), "Frame");
	// Load the terrain (display a debug message as the loading can take some time)
	std::cout << " \nLoad terrain file ..." << std::endl;
	
	//mesh terrain_mesh = mesh_primitive_quadrangle({ -10, -10, 0 }, { 10, -10, 0 }, { 10, 10, 0 }, { -10, 10, 0 });
	mesh terrain_mesh = create_terrain_mesh(100, 100);
	mesh terrain_meshx = create_terrain_mesh(100, 100);
	mesh terrain_meshy = create_terrain_mesh(100, 100);
	mesh terrain_meshxy = create_terrain_mesh(100, 100);


	terrain.initialize(terrain_mesh, "Terrain");
	terrainx.initialize(terrain_meshx, "TerrainX");
	terrainx.anisotropic_scale.x = -1.0f;
	terrainy.initialize(terrain_meshy, "TerrainY");
	terrainy.anisotropic_scale.y = -1.0f;
	terrainxy.initialize(terrain_meshxy, "TerrainXY");
	terrainxy.anisotropic_scale.x = -1.0f;
	terrainxy.anisotropic_scale.y = -1.0f;


	demilune.initialize(mesh_load_file_obj("assets/demilune.obj"), "Demi-lune");
	terrain.shading.phong.specular = 0.0f;
	GLuint const grass = opengl_load_texture_image("assets/texture_grass.jpg", GL_REPEAT, GL_REPEAT);
	terrain.texture = grass;

	skybox.initialize("assets/skybox/");
	skybox.transform.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, Pi / 2.0f);

	initialize_demilune();

	std::cout << " [OK] Terrain loaded\n" << std::endl;

	// Load the texture of the terrain
	std::cout << " \nLoad terrain texture ..." << std::endl;
	//terrain.texture = opengl_load_texture_image("assets/mountain.jpg");
	std::cout << " [OK] Texture loaded\n" << std::endl;

	// Initial placement of the camera
	environment.camera.position_camera = { 0.0f, 0.0f, 2.0f };
	environment.camera.manipulator_rotate_roll_pitch_yaw(0, Pi / 2.0f, 0);

	demilune.transform.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, Pi / 2.0f);


	//Initialize the terrain

}


int scene_structure::get_matrix_coordinate(float x) {
	if (x + 50 < 0) {
		return(int(x - 50) / 100);
	}
	return(int(x + 50) / 100);
}




void scene_structure::display_terrain(float x, float y, scene_environment_player_head environment) {

	mat3 situation = get_mirroring(x, y);
	std::cout << "Situation" << std::endl;
	for (int i = 0; i < 3; i++) { for (int j = 0; j < 3; j++) { std::cout << situation[i][j] << ""; } std::cout << "\n"; }
	std::cout << int(x) << std::endl;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			int a = situation[i][j];
			vec3 translation = { (2 - i - 1) * 100.0f + 100.0f * get_matrix_coordinate(x), (2 - j - 1) * 100.0f + 100.0f * get_matrix_coordinate(y), 0.0f };
			if (a == 0) {
				terrain.transform.translation = translation;
				draw(terrain, environment);
			}
			if (a == 1) {
				terrainy.transform.translation = translation;
				draw(terrainy, environment);
			}
			if (a == 2) {
				terrainx.transform.translation = translation;
				draw(terrainx, environment);
			}
			if (a == 3) {
				terrainxy.transform.translation = translation;
				draw(terrainxy, environment);
			}
		}
	}

}



mat3 scene_structure::get_mirroring(float x, float y) {
	mat3 m;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (get_matrix_coordinate(x -(i-1)*100) % 2 == 0) {
				if (get_matrix_coordinate(y - (j - 1) * 100) % 2 == 0) {
					m[i][j] = 0;
				}
				else {
					m[i][j] = 1;
				};
			}
			else {
				if (get_matrix_coordinate(y - (j - 1) * 100) % 2 == 0) {
					m[i][j] = 2;
				}
				else {
					m[i][j] = 3;
				}
			};
		}
	}
	return(m);
}


void scene_structure::display()
{	
	// set the light position to the camera
	environment.light = environment.camera.position(); 
	environment.speed = speed;
	environment.c = c;
	//std::cout << environment.c << "c \n";


	// The standard frame
	if (gui.display_frame)
		draw(global_frame, environment);
	
	// Displaying the shape deformed by the shader
	//draw(skybox, environment);

	demilune.update_local_to_global_coordinates();
	draw(demilune, environment);
	display_terrain(environment.camera.position_camera.x, environment.camera.position_camera.y, environment);
	draw(demilune, environment);
}



void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	//ImGui::SliderFloat("Speed", &gui.speed, 0.2f, 5.0f); // Set the camera velocity
}


void opengl_uniform(GLuint shader, scene_environment_player_head const& environment)
{
	// Basic uniform parameters
	opengl_uniform(shader, "projection", environment.projection.matrix());
	opengl_uniform(shader, "view", environment.camera.matrix_view());
	opengl_uniform(shader, "light", environment.light);
	opengl_uniform(shader, "speed", environment.speed);
	opengl_uniform(shader, "c", environment.c);
}