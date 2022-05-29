#pragma once

/** This file contains the custom function signature and structure associated the scene 
*  The (*) indicates elements that are specific to the use of the camera mode  */

#include "cgp/cgp.hpp"
#include "event.hpp"
#include "terrain.hpp"
#include "environment_camera_head.hpp"
#include <iostream>
#include "scene.hpp"




// The element of the GUI that are not already stored in other structures

// The structure of the custom scene
struct player_mover {

	scene_structure* scene;

	//cgp::timer_basic timer; // A basic timer for the camera animation
	cgp::timer_basic timer;

	cgp::vec2 mouse_pos;
	double offset;

	// Movement state variables

	bool isMovingForward = false;
	bool isMovingBackward = false;
	bool isMovingLeftward = false;
	bool isMovingRightward = false;
	bool isJumping = false;
	bool isCrouching = false;

	// Movement physical parameters

	cgp::vec3 speed;
	cgp::vec3 pos;
	float speed_max = 10.0f;
	float walk_acc = 40.0f;
	float jump_acc = 40;
	float f = walk_acc / speed_max;
	float g = 10;

	bool allowJump = true;
	float jumpDrain = 0;
	float t_f = 0;

	void initialize(scene_structure* s);
	void update_mouse(float xpos, float ypos);
	void update_camera();
	void update_c(double o);
	bool isGrounded();
	bool isNearGround();
};





