#pragma once
#include <queue>
#include "cgp/cgp.hpp"

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
	rel_timer timer;
	std::queue<event> event_queue;

	//Functions
	void push_event(int a);
	void update(cgp::vec3 playerPos, cgp::vec3 playerSpeed, float c);
	virtual void activate(int id) = 0;
	//void display(scene_environment_player_head environment);
};


struct lamp : public virtual events {

	cgp::mesh_drawable light_source;
	std::string name;
	float period;
	bool status; //True = on, False = off
	float offset;
	int n = 1;

	//lamp(cgp::vec3 p, std::string light_name, float per);
	void initialize(cgp::vec3 p, std::string light_name, float per);
	void activate(int id);
	cgp::mesh_drawable get_mesh();
};
