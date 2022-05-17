#include "event.hpp"


using namespace cgp;


// RELATIVISTIC TIMER

rel_timer::rel_timer(cgp::vec3 speed, float c) {
	float beta = norm(speed) / c;
	gamma = 1 / sqrt(1 - beta * beta);
}

float rel_timer::update() {
	float dt = c_timer.update();
	t += gamma * dt;
	return gamma * dt;
}


// EVENT GESTION

event::event(float cd, int i)
{
	creation_date = cd;
	id = i;
}



void events::push_event(int a) {
	event_queue.push(event(timer.t, a));
}

void events::update(vec3 playerPos, float c) {
	float d = norm(pos - playerPos);
	bool cont = false;
	if (!event_queue.empty()) {
		do {
			event e = event_queue.front();
			float delta_t = timer.t - e.creation_date;
			if (delta_t * c > d)
			{
				activate(e.id);
				cont = true;
				event_queue.pop();
			}
		} while (cont);
	}
}


// LIGHT

void light::initialize(vec3 p, std::string light_name, float per, float o) {
	o = 0;
	per = 1;
	pos = p;
	period = per;
	name = light_name;
	status = false;
	offset = o;
	mesh sphere_mesh = mesh_primitive_sphere();
	light_source.initialize(sphere_mesh, light_name);
	light_source.transform.translation = pos;
	light_source.shading.color = { 1.0f, 1.0f, 1.0f };
}

cgp::mesh_drawable light::display_light() {

	if (timer.t > period) {
		timer.t -= period;
		if (status)
			push_event(0);
		else
			push_event(1);
		status = !status;
	}
	return(light_source);
}

void light::activate(int id) {
	if (id == 0) {
		light_source.shading.color = { 0.0f, 0.0f, 0.0f };
	}
	else {
		light_source.shading.color = { 1.0f, 1.0f, 1.0f };
	}
}