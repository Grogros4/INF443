#pragma once
#include <queue>
#include "cgp/cgp.hpp"
#include "key_positions_structure.hpp"
#include "interpolation.hpp"




struct rel_timer {
	float gamma;
	float t;
	cgp::timer_basic c_timer;

	float update(cgp::vec3 speed, float c);
};

struct event {
	float creation_date;
	int id;
	cgp::vec3 event_pos;
	cgp::vec3 event_speed;

	event(float cd, int i, cgp::vec3 pos);
	event(float cd, int i, cgp::vec3 pos,cgp::vec3 speed);
};


struct events {

	cgp::vec3 pos;
	cgp::timer_basic timer;
	std::queue<event> event_queue;

	//Functions
	void push_event(int a);
	void push_event(int a, cgp::vec3 position, cgp::vec3 speed);
	void update(cgp::vec3 playerPos, cgp::vec3 playerSpeed, float c);
	virtual void activate(int id, cgp::vec3 e_position, cgp::vec3 e_speed) = 0;
	//void display(scene_environment_player_head environment);
};


struct lamp : public virtual events {

	cgp::hierarchy_mesh_drawable light_source_on;
	cgp::hierarchy_mesh_drawable light_source_off;
	std::string name;
	float period;
	bool status; //True = on, False = off
	bool current_status;
	float offset;
	rel_timer clock;

	//lamp(cgp::vec3 p, std::string light_name, float per);
	void initialize(cgp::vec3 p, std::string light_name, float per);
	void activate(int id, cgp::vec3 e_position, cgp::vec3 e_speed);
	cgp::hierarchy_mesh_drawable get_mesh(cgp::vec3 speed, float c);
};


struct car : public virtual events {

	cgp::timer_interval key_timer;
	cgp::mesh_drawable body;
	cgp::vec3 current_pos;
	cgp::vec3 current_speed;
	keyframe_structure keyframes;

	void initialize(cgp::mesh_drawable car_body, cgp::buffer<cgp::vec3> const& key_positions, cgp::buffer<float> const& key_times);
	void activate(int id, cgp::vec3 e_position, cgp::vec3 e_speed);
	cgp::mesh_drawable get_mesh();
};