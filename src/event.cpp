
#include "terrain.hpp"
#include "scene.hpp"
#include "event.hpp"


using namespace cgp;

struct event {
	float creation_date;
	int id;

	event(float cd, int i)
	{
		creation_date = cd;
		id = i;
	}
};

void events::push_event(int a) {
	event_queue.push(a);
}



/*
bool events::is_visible(float x, float y, float z, float c) {
	float rayon = c * global_time;
	return((x - pos.x) * (x - pos.x) + (y - pos.y * (y - pos.y) + (z - pos.z) * (z - pos.z) > rayon * rayon));
}

void events::update_rayon(float time) {
	global_time = global_time + time;
}


void lights::initialize_light(float x, float y, float z, std::string light_name, float f) {
	pos = { x,y,z };
	frequency = f;
	name = light_name;
	clock = 0;
	status = false;
	mesh sphere_mesh = mesh_primitive_sphere();
	light_source.initialize(sphere_mesh, light_name);
	light_source.transform.translation = pos;
	light_source.shading.color = { 1.0f, 1.0f, 1.0f };
}

void lights::update_clock(float time) {
	clock = clock + time;
	if (clock >= frequency) {
		clock = 0;
		status =  !status;
		event_queue.pop();
	}
}

void lights::update_color() {
	if (status) {
		light_source.shading.color = { 0.0f, 0.0f, 0.0f };
	}
	else {
		light_source.shading.color = { 1.0f, 1.0f, 1.0f };
	}
}	
*/