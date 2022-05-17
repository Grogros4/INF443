
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

event::event(float cd, int i)
{
	creation_date = cd;
	id = i;
}

void events::push_event(int a) {
	event_queue.push(event(timer.t, a));
}

void events::update(vec3 playerPos, vec3 playerSpeed, float c) {
	float d = norm(pos - playerPos);
	bool cont = false;
	do {
		if (!event_queue.empty()) {
			event e = event_queue.front();
			float delta_t = timer.t - e.creation_date;
			if (delta_t * c > d)
			{
				std::cout << timer.t << std::endl;
				std::cout << e.id << std::endl;
				activate(e.id);
				cont = true;
				event_queue.pop();
			}
			else {
				cont = false;
			}
		}
		else {
			cont = false;
		}
	} while (cont);
		}


// LAMP

void lamp::initialize(vec3 p, std::string light_name, float per) {
	pos = p;
	period = per;
	name = light_name;
	status = false;
	offset = 0.0f;


	//Creation of the lamp structure
	mesh_drawable lampadaire;
	lampadaire.initialize(mesh_load_file_obj("assets/objects/lampadaire.obj"), "base" + name);
	lampadaire.transform.scaling = 0.5f;
	lampadaire.shading.color = { 0,0,0 };
	lampadaire.transform.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, Pi / 2.0f);


	mesh_drawable sphere;
	sphere.initialize(mesh_primitive_sphere(0.7f), "sphere" + name);
	sphere.shading.color = { 1.0f, 1.0f, 1.0f };
	sphere.transform.translation = {0, 0, 9};

	light_source_on.add(lampadaire);
	light_source_on.add(sphere, "base" + name);
	light_source_on["base" + name].transform.translation = pos;
	light_source_on["base" + name].transform.scaling = 0.4f;
	light_source_on.update_local_to_global_coordinates();


	sphere.shading.color = { 0, 0,0 };
	light_source_off.add(lampadaire);
	light_source_off.add(sphere, "base" + name);
	light_source_off["base" + name].transform.translation = pos;
	light_source_off["base" + name].transform.scaling = 0.4f;
	light_source_off.update_local_to_global_coordinates();
}

cgp::hierarchy_mesh_drawable lamp::get_mesh(vec3 speed, float c) {
	timer.update();
	clock.update(speed, c);
	if (clock.t > period) {
		std::cout << "Creation" << std::endl;
		clock.t -= period;
		if (status)
			push_event(0);
		else
			push_event(1);
		status = !status;
	}
	if (current_status){
		return light_source_off;
	}
	else {
		return light_source_on;
	};
}


void lamp::activate(int id) {
	std::cout << id << std::endl;
	if (id == 0) {
		current_status = false;
	}
	else {
		current_status = true;
	}
}
