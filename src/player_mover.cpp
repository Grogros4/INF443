#include "player_mover.hpp"


using namespace cgp;


player_mover::player_mover(scene_structure* s)
{
	scene = s;
}

void player_mover::update_mouse(float xpos, float ypos)
{
	vec2 new_pos = vec2{xpos, ypos};
	mouse_pos = new_pos;
}

void player_mover::update_c(double o)
{
	if (o > offset)
	{
		scene->c += (scene->c - scene->speed_max - 0.1) * 0.1;
	}
	else
	{
		scene->c -= (scene->c - scene->speed_max - 0.1) * 0.1;
	}
}

void player_mover::update_camera()
{
	camera_head& camera = scene->environment.camera;

	vec3 old_pos = camera.position_camera;

	float const dt = timer.update();

	float const pitch = 0.001f; // speed of the pitch
	float const yaw = 0.001f; // speed of the yaw

	float pitch_angle = - pitch * mouse_pos.y;
	float yaw_angle = - yaw * mouse_pos.x;

	// Orientation of the camera depending on cursor position
	camera.manipulator_rotate_roll_pitch_yaw(0, Pi / 2.0f, 0);
	camera.orientation_camera = rotation_transform::convert_axis_angle_to_quaternion({ 0,0,1 }, yaw_angle);
	if ( !(camera.front().z < -0.99 && pitch_angle < 0) && !(camera.front().z > 0.99 && pitch_angle > 0) )
		camera.manipulator_rotate_roll_pitch_yaw(0, pitch_angle, 0);
	quaternion rot = camera.orientation_camera.data;
	camera.orientation_camera = rotation_transform::convert_axis_angle_to_quaternion({ 0,0,1 }, yaw_angle) * rot;

	// Player acceleration
	vec3 acc = vec3{ 0.0f, 0.0f, 0.0f };

	// Unit vector giving the orientation of the camera in the plane z=0
	vec3 front = vec3{ camera.front().x, camera.front().y, 0 };
	front /= norm(front);

	// Calculating acceleration depending on keyboard inputs
	if (isMovingForward) acc += front;
	if (isMovingBackward) acc += - front;
	if (isMovingRightward) acc += camera.right();
	if (isMovingLeftward) acc += - camera.right();
	if (norm(acc) > 0.01)
		acc = (walk_acc / norm(acc)) * acc;

	bool grounded = isGrounded();

	// Gravity effects
	float g_eff = grounded ? 0 : g;

	// Jump management
	if (grounded && isJumping)
		speed.z = 5.0f;

	// Updating speed and position
	speed += acc * dt - f * vec3{ speed.xy(), 0} *dt + g_eff * vec3{0, 0, -1} *dt;
	camera.position_camera += speed * dt;

	int scalex;
	int scaley;
	mat3 situation = scene->get_mirroring(pos.x, pos.y);

	if (situation[0][0] == 0) {
		scalex = 1;
		scaley = 1;
	}
	if (situation[0][0] == 1) {
		scalex = 1;
		scaley = -1;
	}
	if (situation[0][0] == 2) {
		scalex = -1;
		scaley = 1;
	}
	if (situation[0][0] == 3) {
		scalex = -1;
		scaley = -1;
	}
	float u = pos.x - scene->get_matrix_coordinate(pos.x) * scene->chunk_size;
	float v = pos.y - scene->get_matrix_coordinate(pos.y) * scene->chunk_size;
	u = scalex * u;
	v = scaley * v;
	//if (camera.position_camera.z < evaluate_hills_height(u, v, scene->chunk_size) + 1.5)
	//	camera.position_camera.z = evaluate_hills_height(u, v, scene->chunk_size) + 1.5;

	if (grounded && !isJumping)
		camera.position_camera.z = camera.position_camera.z = evaluate_hills_height(u, v, scene->chunk_size) + 1.5;


	pos = camera.position();

	scene->environment.camera = camera;
	scene->pos = pos;
	scene->speed = (pos - old_pos) /dt;
}

bool player_mover::isGrounded()
{
	int scalex;
	int scaley;
	mat3 situation = scene->get_mirroring(pos.x, pos.y);

	if (situation[0][0] == 0) {
		scalex = 1;
		scaley = 1;
	}
	if (situation[0][0] == 1) {
		scalex = 1;
		scaley = -1;
	}
	if (situation[0][0] == 2) {
		scalex = -1;
		scaley = 1;
	}
	if (situation[0][0] == 3) {
		scalex = -1;
		scaley = -1;
	}
	
	float u = pos.x - scene->get_matrix_coordinate(pos.x) * scene->chunk_size;
	float v = pos.y - scene->get_matrix_coordinate(pos.y) * scene->chunk_size;
	if (pos.z - 1.5 - evaluate_hills_height(scalex*u, scaley*v, scene->chunk_size) < 0.5)
	{
		return true;
	}
	return false;
}