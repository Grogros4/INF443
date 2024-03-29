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
	demi_lunes.initialize(&environment, &terrain);

	// Initializing lamp grid
	buffer<vec2> lamp_positions = { {3.76,-3.38},{3.7,4.8},{25.9,9.5},{11.2,27.4},{-0.3,28.5},{25.5,54},{56.2,43.3}, {93.3, 13.0}, {108.5,52 }, {30, -69},{64,-18},  {190.4, 50} ,{190.4, 40}, {190.4, 30}, {190, 20}, {191, 10}, {191, 0}, {190, -10}, {189, -20}, {188, -30}, {188, -40}, {188, -50}, };
	int i = 0;
	float t = 0; //Ensures that all lights are synchronized
	for (vec2 v : lamp_positions)
	{
		lamp l;
		l.initialize(&environment, vec3{ v.x, v.y, terrain.evaluate_hills_height(v.x,v.y) + 0.1 }, "lamp" + std::to_string(i), 0.5f);
		l.clock.t = t;
		lamp_list.push_back(l);
		lamp_list.front().clock.update({ 0,0,0 }, 40);
		t = lamp_list.front().clock.t;
		i += 1;
	}



	// Initializing sky
	sky.initialize(&environment, 400, 1000, 0.5);

	// Initializing the car
	 
	// Key 3D positions

	key_positions = { {164.5,-140.9,1}, {164.5,-140.90,1}, {180.68,-48,1}, {185.79,15.1,1}, {181.95,90.7,1}, {173.1,199,1}, {164.2,252,1},{164.2,252,1} };
	
	//This part computes automaticaaly the key_times based on the wanted speed (here 9.0 because c > 10)
	float wanted_speed = 9.0f;
	t = 1.0f;
	vec3 prev_v = { 164.5,0,-140.9 };
	key_times = {};
	for (vec3 v : key_positions) {
		t += norm(v - prev_v) / wanted_speed + 0.001f;
		key_times.push_back(t);
		prev_v = v;
	}

	//This part initialize the car mesh drawable
	car_mesh.initialize(mesh_load_file_obj("assets/car/car.obj"));
	rotation_transform rot;
	rot = rotation_transform::from_axis_angle({ 1,0,0 }, Pi / 2);
	rot = rot * rotation_transform::from_axis_angle({ 0,1,0 }, Pi);
	car_mesh.transform.rotation = rot;
	car_mesh.shading.color = { 0,0,1 };


	// Initial placement of the camera
	environment.camera.position_camera = { 182.0f, 90.0f, 2.0f };
	environment.camera.manipulator_rotate_roll_pitch_yaw(0, Pi / 2.0f, 0);

	//Initializing the ortho camera
	environment_hud.projection = camera_projection::orthographic(-1, 1, -1, 1, -1, 1);
	environment_hud.light = { 0,0,1 };
	environment_hud.light_speed = 30000000.0f;
	environment_hud.speed = { 0,0,0 };
	environment_hud.obj_speed = { 0,0,0 };


	// Initialize HUD texture

	//Intizalizing clock
	quad.initialize(mesh_primitive_disc(0.2, { 1.6,-0.6,-0.01f } , { 0,0,1 }, 100), "Quad");
	quad.texture = opengl_load_texture_image("assets/clock/clock.png");
	second.initialize(mesh_primitive_quadrangle({ -0.001,0,0 }, { -0.001,0.16,0 }, { 0.001,0.16,0 }, { 0.001,0,0 }), "Second");
	second.transform.translation = { 1.6,-0.6,0 };
	second.shading.color = { 1,0,0 };

	//Initializing speed_meter
	meter.initialize(mesh_primitive_disc(0.2, { -1.6, -0.6, -0.01f }, { 0,0,1 }, 100), "Meter");
	meter.texture = opengl_load_texture_image("assets/c bar/compteur.png");
	meter_bar.initialize(mesh_primitive_quadrangle({ -0.001,0,0 }, { -0.001,0.16,0 }, { 0.001,0.16,0 }, { 0.001,0,0 }), "Meter_bar");
	meter_bar.transform.translation = { -1.6, -0.6, 0 };
	meter_bar.shading.color = { 1,0,0 };

	//Intializing c_meter
	cmeter.initialize(mesh_primitive_disc(0.2, { -1.1,-0.6,-0.01f }, { 0,0,1 }, 100), "Meter");
	cmeter.texture = opengl_load_texture_image("assets/c bar/c_compteur.png");
	cmeter_bar.initialize(mesh_primitive_quadrangle({ -0.001,0,0 }, { -0.001,0.16,0 }, { 0.001,0.16,0 }, { 0.001,0,0 }), "Meter_bar");
	cmeter_bar.transform.translation = { -1.1,-0.6,0 };
	cmeter_bar.shading.color = { 1,0,0 };

	//Used to output coordinates to the console every second
	previous_time = 0;
}


void scene_structure::display()
{	

	// Updating world parameters
	environment.light = vec3{0, 0, 50};
	environment.speed = speed;
	environment.light_speed = c;
	environment.obj_speed = { 0,0,0 };
	environment.stellar_aberration = gui.stellar_aberration;
	environment.doppler_effect = gui.doppler_effect;
	environment.relativistic_brightness = gui.relativistic_brightness;
	environment.hud = gui.hud;
	environment.event_delay = gui.event_delay;

	// Displaying terrain (and trees)
	terrain.display();

	// Displaying sky (and stars)
	sky.display();

	// Displaying demilunes
	demi_lunes.display();

	// Updating and displaying lamps
	for (lamp& l : lamp_list)
	{
		l.update(pos, speed, c);
	}

	if (gui.hud)
	{
		// Updating and displaying clock HUD
		clock_timer.update(speed, c);
		draw(quad, environment_hud);
		second.transform.rotation = rotation_transform::from_axis_angle({ 0,0,1 }, -(int(clock_timer.t) % 60) * Pi / 30);
		draw(second, environment_hud);

		//Updating and displaying speed_meter HUD
		draw(meter, environment_hud);
		meter_bar.transform.rotation = rotation_transform::from_axis_angle({ 0,0,1 }, (5 - (norm(speed) / c) * 10) * Pi / 6);
		draw(meter_bar, environment_hud);

		//Updating and displaying speed_meter HUD
		draw(cmeter, environment_hud);
		if (log(10) - log(c) <= -10 * Pi / 6) {
			std::cout << "Max c reached" << std::endl;
			c = exp(10 * Pi / 6) * 10;
		}
		cmeter_bar.transform.rotation = rotation_transform::from_axis_angle({ 0,0,1 }, log(10) - log(c) + 5 * Pi / 6);
		draw(cmeter_bar, environment_hud);
	}

	// Updating and displaying car
	
	//Adding car sequentially
	if ( (clock_timer.c_timer.t > 15) && (car_list.size() == 0)) {
		add_car();
	}	

	if ((clock_timer.c_timer.t > 30) && (car_list.size() == 1)) {
		add_car();
	}

	if ((clock_timer.c_timer.t > 45) && (car_list.size() == 2)) {
		add_car();
	}

	if ((clock_timer.c_timer.t > 65) && (car_list.size() == 3)) {
		add_car();
	}

	if ((clock_timer.c_timer.t > 75) && (car_list.size() == 4)) {
		add_car();
	}
	
	//Updating cars
	for (car& car : car_list)
	{
		car.update(pos, speed, c, terrain);
	}

	display_gui();

	//Send coordinates to the console
	if (previous_time < clock_timer.t - 1) {
		std::cout << "Current position" << std::endl;
		std::cout << "x : ";
		std::cout << pos.x << std::endl;
		std::cout << "y :";
		std::cout << pos.y << std::endl;
		std::cout << "z :";
		std::cout << pos.z << std::endl;
		previous_time = clock_timer.t;
	}
}

void scene_structure::display_gui()
{
	ImGui::Checkbox("Stellar Aberration", &gui.stellar_aberration);
	ImGui::Checkbox("Doppler Effect", &gui.doppler_effect);
	ImGui::Checkbox("Relativistic Brightness", &gui.relativistic_brightness);
	ImGui::Checkbox("HUD", &gui.hud);
	ImGui::Checkbox("Event Delay", &gui.event_delay);
}

void scene_structure::add_car() {
	car car0;
	car0.initialize(&environment, car_mesh, key_positions, key_times);
	car_list.push_back(car0);
	std::cout << "done" << std::endl;
}