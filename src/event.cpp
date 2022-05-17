
#include "event.hpp"


using namespace cgp;


// RELATIVISTIC TIMER

float rel_timer::update(vec3 speed, float c) {
	float beta = norm(speed) / c;
	gamma = 1 / sqrt(1 - beta * beta);
	float dt = c_timer.update();
	t += gamma * dt;
	return gamma * dt;
}


// EVENT GESTION

single_event::single_event(float cd, int i)
{
	creation_date = cd;
	id = i;
}

void multiple_events::push_event(int a) {
	event_queue.push(single_event(timer.t, a));
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


// LAMP

void lamp::initialize(vec3 p, std::string light_name, float per) {
	pos = p;
	period = per;
	name = light_name;
	status = false;
	offset = 0.0f;
	mesh sphere_mesh = mesh_primitive_sphere();
	light_source.initialize(sphere_mesh, light_name);
	light_source.transform.translation = pos;
	light_source.shading.color = { 1.0f, 1.0f, 1.0f };
}


mesh_drawable lamp::get_mesh() {

	if (timer.t > period) {
		timer.t -= period;
		if (status)
			push_event(0);
		else
			push_event(1);
		status = !status;
	}

	return light_source;
}

void lamp::activate(int id) {
	if (id == 0) {
		light_source.shading.color = { 0.0f, 0.0f, 0.0f };
	}
	else {
		light_source.shading.color = { 1.0f, 1.0f, 1.0f };
	}
}
