#pragma once

/** This file contains the custom function signature and structure associated the scene 
*  The (*) indicates elements that are specific to the use of the camera mode  */

#include "cgp/cgp.hpp"
#include "event.hpp"
#include "environment_camera_head/environment_camera_head.hpp"
#include <iostream>


// The element of the GUI that are not already stored in other structures
struct gui_parameters {
	bool display_frame = false;
};

struct scene_environment_player_head : public scene_environment_camera_head {
	cgp::vec3 speed;
	float c;
};


float c = 13.0f;
cgp::vec3 speed;


// The structure of the custom scene
struct scene_structure {
	
	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //

	scene_environment_player_head environment; // The specific scene environment that contains a "head camera" (*)
	
	cgp::mesh_drawable global_frame;    // The standard global frame
	cgp::mesh_drawable terrain;         // The terrain loaded from an external file
	cgp::skybox_drawable skybox;

	cgp::hierarchy_mesh_drawable demilune;
	cgp::mesh_drawable terrainx;
	cgp::mesh_drawable terrainy;
	cgp::mesh_drawable terrainxy;

	light l1;

	cgp::timer_basic timer; // A basic timer for the camera animation

	gui_parameters gui;     // The standard GUI element storage
	cgp::inputs_interaction_parameters inputs; // Storage for inputs status (mouse, keyboard, window dimension)

	cgp::vec3 pos;
	bool isJumping = false;
	bool fly_mode = false;
	float speed_max = 10.0f;
	float walk_acc = 40.0f;
	float f = walk_acc / speed_max;
	float g = 10;
	int chunk_size = 100;

	cgp::vec2 mouse_pos;
	cgp::vec2 mouse_speed;

	
	// ****************************** //
	// Functions
	// ****************************** //
	
	// Function to call in the animation loop in main (*)
	//   This function replace the standard trackball behavior that must also be removed in the main (from mouse_move_callback)
	void update_camera(float xpos, float ypos);
	bool isGrounded();

	void initialize_demilune();
	void initialize();  // Standard initialization to be called before the animation loop
	void display();     // The frame display to be called within the animation loop
	void display_gui(); // The display of the GUI, also called within the animation loop 
	void display_terrain(float x, float y, scene_environment_player_head environment);
	int get_matrix_coordinate(float x);
	cgp::mat3 get_mirroring(float x, float y);
};





