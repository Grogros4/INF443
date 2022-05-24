#pragma once

#include "cgp/cgp.hpp"
#include "environment_camera_head.hpp"

struct Sky {

	scene_environment_camera_head* environment;

	cgp::mesh_drawable sky_sphere;
	cgp::mesh_drawable star_sphere;
	float skyRadius;
	int N;
	float starRadius;

	cgp::vec3 sky_color = cgp::vec3{ 0, 0, 0 };
	cgp::vec3 star_color = cgp::vec3{ 1, 1, 1 };

	std::vector<cgp::vec3> stars_positions;

	void initialize(scene_environment_camera_head* env, float R, int n, float r);
	std::vector<cgp::vec3> generate_positions_on_sky();
	void display();
};