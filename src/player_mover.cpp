#include "player_mover.hpp"


using namespace cgp;


void PlayerMover::initialize(scene_structure* s)
{
	scene = s;
	//pos = scene->environment.camera.position_camera;
	pos = vec3{ 182.0f, 90.0f, 0.0f };
}

void PlayerMover::update_mouse(float xpos, float ypos)
{
	vec2 new_pos = vec2{xpos, ypos};
	mouse_delta = new_pos - mouse_pos;
	mouse_pos = new_pos;
}

void PlayerMover::update_c(double o)
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

void PlayerMover::update_camera()
{

	camera_head& camera = scene->environment.camera;

	float const dt = timer.update();

	float beta = norm(scene->speed) / scene->c;
	float gamma = 1 / sqrt(1 - beta * beta);

	float const pitch = 0.00001f; // speed of the pitch
	float const yaw = 0.00001f; // speed of the yaw

	float pitch_angle = - pitch * mouse_delta.y / dt;
	float yaw_angle = - yaw * mouse_delta.x / dt;
	mouse_delta = vec2{ 0, 0 };

	// Orientation of the camera depending on cursor position
	if (!(camera.front().z < -0.99 && pitch_angle < 0) && !(camera.front().z > 0.99 && pitch_angle > 0))
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
	bool nearGround = isNearGround();

	// Gravity effects
	float g_eff = grounded ? 0 : g;

	// Jump management
	if (nearGround && isJumping && allowJump)
		g_eff = - jump_acc;
	if (nearGround && !allowJump && timer.t < t_f)
		g_eff = - jumpDrain;
	if ((grounded) && nearGround && !isJumping)
	{
		speed.z = 0;
		//allowJump = true;
		g_eff = 0;
		pos.z = scene->terrain.evaluate_hills_height(pos.x, pos.y) + 1.5;
	}

	// Acceleration in the referenrial of the player
	acc += -f * vec3{ speed.xy(), 0 } + g_eff * vec3{ 0, 0, -1 };

	// Acceleration in the referential of the world
	acc = acc / (gamma * gamma * gamma);

	// Updating speed and position in the referential of the world, according to the time of the player
	speed += acc * dt * gamma;
	pos += speed * dt * gamma;

	//if (grounded && !isJumping)
	//	pos.z = scene->terrain.evaluate_hills_height(pos.x, pos.y) + 1.5;

	if (isGrounded() && !allowJump)
		speed.z = 0;
	if (isGrounded())
		allowJump = true;
	if (nearGround && !isNearGround())
		allowJump = false;
	if (!nearGround && isNearGround())
	{
		float h = pos.z - 1.5 - scene->terrain.evaluate_hills_height(pos.x, pos.y);
		h += 0.5;
		jumpDrain = gamma * speed.z * speed.z / h;
		t_f = timer.t - h / speed.z;
	}

	camera.position_camera = pos;

	scene->environment.camera = camera;
	scene->pos = pos;
	//scene->speed = (pos - old_pos) / dt;
	scene->speed = speed;
}

bool PlayerMover::isGrounded()
{
	if (pos.z - 1.5 - scene->terrain.evaluate_hills_height(pos.x, pos.y) < 0.01)
	{
		return true;
	}
	return false;
}

bool PlayerMover::isNearGround()
{
	if (pos.z - 1.5 - scene->terrain.evaluate_hills_height(pos.x, pos.y) < 0.5)
	{
		return true;
	}
	return false;
}