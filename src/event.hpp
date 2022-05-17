#pragma once

#include "cgp/cgp.hpp"

struct events {
	float rayon;
	cgp::vec3 pos;

	//Functions
	bool is_visible(float x, float y, float z);
	void update_rayon(float time, float c);
};