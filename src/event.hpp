#pragma once
#include <queue>
#include "cgp/cgp.hpp"


struct event {
	float creation_date;
	int id;

	event event(float a, int n);
};




struct events {
	cgp::vec3 pos;
	cgp::timer_basic timer;
	std::queue<event> event_queue;

	//Functions
	void push_event(int a);
	void activate();
};


struct lights : public events{

	cgp::mesh_drawable light_source;

	std::string name;
	float frequency;
	//private : float clock;
	bool status; //True = on, False = off
	
	cgp::vec3 pos;
	events event_queue;

	void initialize_light(float x, float y, float z, std::string name, float f);
	void update_clock(float time);
	void update_color();
	//cgp::mesh create_light(int r);
};