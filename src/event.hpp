#pragma once
#include <queue>
#include "cgp/cgp.hpp"
#include "environment_camera_head.hpp"


struct rel_timer {
	float gamma;
	float t;
	cgp::timer_basic c_timer;

	float update(cgp::vec3 speed, float c);
};

struct event {
	float creation_date;
	int id;

	event(float cd, int i);
};


struct events {

	cgp::vec3 pos;
	cgp::timer_basic timer;
	std::queue<event> event_queue;

	//Functions
	void push_event(int a);
	void update(cgp::vec3 playerPos, cgp::vec3 playerSpeed, float c);
	virtual void activate(int id) = 0;
	//void display(scene_environment_player_head environment);
};


struct lamp : public virtual events {

	scene_environment_camera_head environment;
	cgp::mesh_drawable lampadaire;
	cgp::mesh_drawable sphere;
	std::string name;
	float period;
	bool status; //True = on, False = off
	bool current_status;
	float offset;
	rel_timer clock;

	//lamp(cgp::vec3 p, std::string light_name, float per);
	void initialize(scene_environment_camera_head& environment, cgp::vec3 p, std::string light_name, float per);
	void activate(int id);
	void update(cgp::vec3 playerPos, cgp::vec3 playerSpeed, float c);
	cgp::hierarchy_mesh_drawable get_mesh(cgp::vec3 speed, float c);
};
