#pragma once

#include "cgp/cgp.hpp"
#include "environment_camera_head.hpp"
#include "tree.hpp"

struct Terrain {

	scene_environment_camera_head* environment;

	Trees trees;

	cgp::mesh_drawable terrain;
	cgp::mesh_drawable terrainx;
	cgp::mesh_drawable terrainy;
	cgp::mesh_drawable terrainxy;

	std::vector<cgp::vec3> tree_position;
	std::vector<cgp::vec3> tree_positionx;
	std::vector<cgp::vec3> tree_positiony;
	std::vector<cgp::vec3> tree_positionxy;

	int chunk_size;

	float evaluate_hills_height(float x, float y);
	cgp::mesh create_terrain_mesh(int N, float scalex, float scaley);
	int get_matrix_coordinate(float x);
	cgp::mat3 get_mirroring(float x, float y);
	void initialize(scene_environment_camera_head* env, int N, float length);
	std::vector<cgp::vec3> generate_positions_on_terrain(int N, float scalex, float scaley);
	void display();
};