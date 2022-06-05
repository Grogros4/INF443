#pragma once

/** This file contains the custom function signature and structure associated the scene 
*  The (*) indicates elements that are specific to the use of the camera mode  */

#include "cgp/cgp.hpp"
#include "event.hpp"
#include "terrain.hpp"
#include "environment_camera_head.hpp"
#include <iostream>
#include "demilunes.hpp"
#include "sky.hpp"
#include <list>

typedef std::list<lamp> LAMPS;
typedef std::list<car> CARS;

// The element of the GUI that are not already stored in other structures
struct gui_parameters {
	bool stellar_aberration = true;
	bool doppler_effect = true;
	bool relativistic_brightness = true;
	bool hud = true;
};

struct scene_environment_hud : public cgp::scene_environment_basic {
	cgp::vec3 env_speed;
	float env_c;
};

// The structure of the custom scene
struct scene_structure {
	
	 // The specific scene environment that contains a "head camera"
	scene_environment_camera_head environment;

	// Specific environment to draw HUD elements
	scene_environment_camera_head environment_hud;
	
	// The standard global frame
	cgp::mesh_drawable global_frame;

	// The standard GUI element storage
	gui_parameters gui;

	// HUD elements
	cgp::mesh_drawable quad;
	cgp::mesh_drawable second;
	rel_timer clock_timer;

	cgp::mesh_drawable meter;
	cgp::mesh_drawable meter_bar;

	cgp::mesh_drawable cmeter;
	cgp::mesh_drawable cmeter_bar;

	// Terrain (including trees)
	Terrain terrain;
	int chunk_size = 100;

	// Sky (including stars)
	Sky sky;

	// 4 demilunes buildings
	Demilunes demi_lunes;
	
	// Lamps
	LAMPS lamp_list;


	// Car
	CARS car_list;
	cgp::buffer<cgp::vec3> key_positions;
	cgp::buffer<float> key_times;
	cgp::mesh_drawable car_mesh;

	// Storage for inputs status (mouse, keyboard, window dimension)
	cgp::inputs_interaction_parameters inputs;

	// Phisical variables required for relativity
	float c = 40.0f;
	cgp::vec3 speed;
	cgp::vec3 pos;
	

	// Display coordinates
	float previous_time;

	void initialize();
	void display();
	void display_gui();
	void add_car();
};





