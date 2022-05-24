#pragma once

/** This file contains the custom function signature and structure associated the scene 
*  The (*) indicates elements that are specific to the use of the camera mode  */

#include "cgp/cgp.hpp"
#include "event.hpp"
#include "terrain.hpp"
#include "environment_camera_head.hpp"
#include <iostream>
#include "demilunes.hpp"




// The element of the GUI that are not already stored in other structures
struct gui_parameters {
	bool display_frame = false;
};

struct scene_environment_hud : public cgp::scene_environment_basic {
	cgp::vec3 env_speed;
	float env_c;
};

// The structure of the custom scene
struct scene_structure {
	
	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //

	scene_environment_camera_head environment; // The specific scene environment that contains a "head camera" (*)
	scene_environment_camera_head environment_hud; // Specific environment to draw HUD elements
	
	cgp::mesh_drawable global_frame;    // The standard global frame
	cgp::skybox_drawable skybox;

	cgp::mesh_drawable quad;
	cgp::mesh_drawable second;
	rel_timer clock_timer;

	cgp::mesh_drawable meter;
	cgp::mesh_drawable meter_bar;

	Terrain terrain;
	Demilunes demi_lunes;


	cgp::mesh_drawable lampadaire;
	lamp l1;
	lamp l2;

	car car1;

	cgp::mesh_drawable sky;


	cgp::timer_basic timer; // A basic timer for the camera animation

	cgp::inputs_interaction_parameters inputs; // Storage for inputs status (mouse, keyboard, window dimension)

	float c = 40.0f;
	cgp::vec3 speed;
	cgp::vec3 pos;

	int chunk_size = 100;

	
	// ****************************** //
	// Functions
	// ****************************** //
	
	// Function to call in the animation loop in main (*)
	//   This function replace the standard trackball behavior that must also be removed in the main (from mouse_move_callback)

	void initialize();  // Standard initialization to be called before the animation loop
	void display();     // The frame display to be called within the animation loop
};





