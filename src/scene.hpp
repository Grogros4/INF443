#pragma once

/** This file contains the custom function signature and structure associated the scene 
*  The (*) indicates elements that are specific to the use of the camera mode  */

#include "cgp/cgp.hpp"
#include "environment_camera_head/environment_camera_head.hpp"


// The element of the GUI that are not already stored in other structures
struct gui_parameters {
	bool display_frame = false;
	float speed = 1.0f;       // The velocity of the camera displacement
};


// The structure of the custom scene
struct scene_structure {
	
	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //

	scene_environment_camera_head environment; // The specific scene environment that contains a "head camera" (*)
	
	cgp::mesh_drawable global_frame;    // The standard global frame
	cgp::mesh_drawable terrain;         // The terrain loaded from an external file

	cgp::timer_basic timer; // A basic timer for the camera animation

	gui_parameters gui;     // The standard GUI element storage
	cgp::inputs_interaction_parameters inputs; // Storage for inputs status (mouse, keyboard, window dimension)

	

	
	// ****************************** //
	// Functions
	// ****************************** //
	
	// Function to call in the animation loop in main (*)
	//   This function replace the standard trackball behavior that must also be removed in the main (from mouse_move_callback)
	void update_camera(); 
	

	void initialize();  // Standard initialization to be called before the animation loop
	void display();     // The frame display to be called within the animation loop
	void display_gui(); // The display of the GUI, also called within the animation loop
};





