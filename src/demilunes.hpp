#pragma once

#include "cgp/cgp.hpp"
#include "terrain.hpp"
#include "environment_camera_head.hpp"
#include <iostream>


struct Demilunes {

	scene_environment_camera_head* environment;

	cgp::hierarchy_mesh_drawable demilune;
	cgp::mesh_drawable path;

	void initialize(scene_environment_camera_head* env);
	void display();
};





