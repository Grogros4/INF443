#pragma once
#include <queue>
#include "cgp/cgp.hpp"


struct event {
	float creation_date;
	int id;
};


struct events {
	float global_time;
	cgp::vec3 pos;
	std::queue<event> event_queue;

	//Functions
	void push_event(event a);
	void activate();
};


struct lights {

	std::string name;
	float frequency;
	private : float clock;
	bool status; //True = on, False = off
	cgp::mesh_drawable light_source; 
	cgp::vec3 pos;
	events event_queue;

	void initialize_light(float x, float y, float z, std::string name, float f);
	void update_clock(float time);
	void update_color();
	//cgp::mesh create_light(int r);
};
