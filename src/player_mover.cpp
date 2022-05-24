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
		scene->c += (scene->c - speed_max) * 0.1;
	}
	else
	{
		scene->c -= (0.97 * scene->c - speed_max) * 0.1;
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
	camera.orientation_camera = rotation_transform::convert_axis_angle_to_quaternion({ 0,0,1 }, yaw_angle);
	if ( !(camera.front().z < -0.99 && pitch_angle < 0) && !(camera.front().z > 0.99 && pitch_angle > 0) )
		camera.manipulator_rotate_roll_pitch_yaw(0, pitch_angle, 0);
	
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
	if (grounded && !isJumping)
		speed.z = 0;

	// Updating speed and position
	speed += acc * dt - f * vec3{ speed.xy(), 0} *dt + g_eff * vec3{0, 0, -1} *dt;
	camera.position_camera += speed * dt;

	//if (camera.position_camera.z < evaluate_hills_height(u, v, scene->chunk_size) + 1.5)
	//	camera.position_camera.z = evaluate_hills_height(u, v, scene->chunk_size) + 1.5;

	if (grounded && !isJumping)
		camera.position_camera.z = camera.position_camera.z = scene->terrain.evaluate_hills_height(pos.x, pos.y) + 1.5;


	pos = camera.position();

	scene->environment.camera = camera;
	scene->pos = pos;
	//scene->speed = (pos - old_pos) / dt;
	scene->speed = speed;
}

bool player_mover::isGrounded()
{
	if (pos.z - 1.5 - scene->terrain.evaluate_hills_height(pos.x, pos.y) < 0.5)
	{
		return true;
	}
	return false;
}