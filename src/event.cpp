
#include "terrain.hpp"
#include "scene.hpp"
#include "event.hpp"


using namespace cgp;

bool events::is_visible(float x, float y, float z) {
	return((x - pos.x) * (x - pos.x) + (y - pos.y * (y - pos.y) + (z - pos.z) * (z - pos.z) > rayon * rayon);
}

void events::update_rayon(float time, float c) {
	rayon = rayon + c * time;
}