#pragma once
#include <queue>
#include "cgp/cgp.hpp"
#include "scene.hpp"


struct rel_timer {
	float gamma;
	float t;
	cgp::timer_basic c_timer;

	float update();
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
	float rel_time;

	//Functions
	void push_event(int a);
	void update(cgp::vec3 playerPos);
	void activate(int id);
	void display(scene_environment_player_head environment);
};


struct light : public events{

	cgp::mesh_drawable light_source;
	std::string name;
	float period;
	bool status; //True = on, False = off
	float offset;

	light(cgp::vec3 p, std::string light_name, float per = 1, float o = 0);
	void activate(int id);
	void display(scene_environment_player_head environment);
};
