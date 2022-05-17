#pragma once
#include <queue>
#include "cgp/cgp.hpp"

struct rel_timer {
	float gamma;
	float t;
	cgp::timer_basic c_timer;

	float update(cgp::vec3 speed, float c);
};

struct single_event {
	float creation_date;
	int id;

	single_event(float cd, int i);
};


struct multiple_events {

	cgp::vec3 pos;
	cgp::timer_basic timer;
	std::queue<single_event> event_queue;

	//Functions
	void push_event(int a);
	void update(cgp::vec3 playerPos, float c);
	virtual void activate(int id) = 0;
	//void display(scene_environment_player_head environment);
};


struct lamp : public virtual multiple_events {

	cgp::mesh_drawable light_source;
	std::string name;
	float period;
	bool status; //True = on, False = off
	float offset;

	//lamp(cgp::vec3 p, std::string light_name, float per);
	void initialize(cgp::vec3 p, std::string light_name, float per);
	void activate(int id);
	cgp::mesh_drawable get_mesh();
};
