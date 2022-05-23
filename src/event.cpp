
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

event::event(float cd, int i, cgp::vec3 pos)
{
	creation_date = cd;
	id = i;
	event_speed = { 0,0,0 };
	event_pos = pos;
}

event::event(float cd, int i, cgp::vec3 pos, cgp::vec3 speed)
{
	creation_date = cd;
	id = i;
	event_pos = pos;
	event_speed = speed;
}


void events::push_event(int a) {
	event_queue.push(event(timer.t, a, pos));
}

void events::push_event(int a, vec3 position, vec3 speed) {
	event_queue.push(event(timer.t, a, position, speed));
}

void events::update(vec3 playerPos, vec3 playerSpeed, float c) {
	float beta;
	float gamma;
	vec3 uve;
	vec3 posEff;
	float d;
	bool cont = false;
	do {
		if (!event_queue.empty()) {
			event e = event_queue.front();
			beta = norm(playerSpeed - e.event_speed) / c;
			gamma = 1 / sqrt(1 - beta * beta);
			uve = (norm(playerSpeed - e.event_speed) < 0.00001) ? vec3{ 0,0,0 } : (playerSpeed - e.event_speed) / norm(playerSpeed - e.event_speed);
			posEff = e.event_pos + ((1 / gamma) - 1) * dot(e.event_pos, uve) * uve;
			d = norm(posEff - playerPos);
			float delta_t = timer.t - e.creation_date;
			if (delta_t * c > d)
			{
				/*
				std::cout << timer.t << std::endl;
				std::cout << e.id << std::endl;
				*/
				activate(e.id, e.event_pos, e.event_speed);
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
	timer.update();
	clock.update(playerSpeed, c);
	if (clock.t > period) {
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


void lamp::activate(int id, cgp::vec3 e_position, cgp::vec3 e_speed) {
	if (id == 0) {
		sphere.shading.color = { 0.0f, 0.0f, 0.0f };
	}
	else {
		sphere.shading.color = { 1.0f, 1.0f, 1.0f };
	}
}


//CAR

void car::initialize(cgp::mesh_drawable car_body, cgp::buffer<cgp::vec3> const& key_positions, cgp::buffer<float> const& key_times) {
	keyframes.initialize(key_positions, key_times);
	body = car_body;
	int N = key_times.size();
	key_timer.t_min = key_times[1];
	key_timer.t_max = key_times[N - 2];
	key_timer.t = key_timer.t_min;
	
}

void car::activate(int id, cgp::vec3 e_position, cgp::vec3 e_speed) {
	current_pos = e_position;
	current_speed = e_speed;
	std::cout << current_pos << std::endl;
	std::cout << current_speed << std::endl;
	
}

cgp::mesh_drawable car::get_mesh() {
	timer.update();
	key_timer.update();
	float t = key_timer.t;
	vec3 p = interpolation(t, keyframes.key_positions, keyframes.key_times);
	int idx = find_index_of_interval(t, keyframes.key_times);
	vec3 speed = (keyframes.key_positions[idx + 1] - keyframes.key_positions[idx]) / (keyframes.key_times[idx + 1] - keyframes.key_times[idx]);
	push_event(0, p, speed);
	body.transform.translation = current_pos;
	return body;
}