
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
	timer.update();
	float beta = norm(playerSpeed) / c;
	float gamma = 1 / sqrt(1 - beta * beta);
	vec3 uve = (norm(playerSpeed) < 0.00001) ? vec3{ 0,0,0 } : playerSpeed / norm(playerSpeed);
	vec3 posEff = pos + ((1 / gamma) - 1) * dot(pos, uve) * uve;
	float d = norm(posEff - playerPos);
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
	lampadaire.initialize(mesh_load_file_obj("assets/objects/lampadaire.obj"), "base_" + name);
	lampadaire.transform.scaling = 0.5f;
	lampadaire.shading.color = { 0,0,0 };
	lampadaire.transform.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, Pi / 2.0f);
	lampadaire.transform.translation = pos;

	sphere.initialize(mesh_primitive_sphere(0.7f), "sphere_" + name);
	sphere.shading.color = { 1.0f, 1.0f, 1.0f };
	sphere.transform.translation = {0, 0, 9};
	sphere.transform.translation += pos;
}

void lamp::update(scene_environment_camera_head env, cgp::vec3 playerPos, cgp::vec3 playerSpeed, float c)
{
	clock.update(playerSpeed, c);
	if (clock.t > period) {
		std::cout << "Creation" << std::endl;
		clock.t -= period;
		if (status)
			push_event(0);
		else
			push_event(1);
		status = !status;
	}

	events::update(playerPos, playerSpeed, c);

	draw(lampadaire, env);
	draw(sphere, env);
}


void lamp::activate(int id) {
	std::cout << id << std::endl;
	if (id == 0) {
		sphere.shading.color = { 0.0f, 0.0f, 0.0f };
	}
	else {
		sphere.shading.color = { 1.0f, 1.0f, 1.0f };
	}
}
