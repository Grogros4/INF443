#pragma once
#include <queue>
#include "cgp/cgp.hpp"




struct rel_timer {
	float gamma;
	float t;
	cgp::timer_basic c_timer;

	rel_timer(cgp::vec3 speed, float c);

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
	void update(cgp::vec3 playerPos, float c);
	virtual void activate(int id) = 0;
	//void display();
};


struct light : public events{

	cgp::mesh_drawable light_source;
	std::string name;
	float period;
	bool status; //True = on, False = off
	float offset;

	void initialize(cgp::vec3 p, std::string light_name, float per, float o);
	void activate(int id);
	cgp::mesh_drawable display_light();
};
