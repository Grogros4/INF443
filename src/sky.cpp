#include "sky.hpp"

using namespace cgp;

const double PI = 3.141592653589793238463;

void Sky::initialize(scene_environment_camera_head* env, float R, int n, float r) {
	environment = env;
	skyRadius = R;
	N = n;
	starRadius = r;

    mesh sky_mesh = mesh_primitive_sphere(R);
    sky_sphere.initialize(sky_mesh, "sky");
    sky_sphere.shading.color = sky_color;
    sky_sphere.shading.phong = shading_parameters_phong::phong_parameters{ 1, 1, 0, 1000 };

    mesh star_mesh = mesh_primitive_sphere(r);
    star_sphere.initialize(star_mesh, "star");
    star_sphere.shading.color = star_color;
    star_sphere.shading.phong = shading_parameters_phong::phong_parameters{ 1, 1, 0, 1000 };

    stars_positions = generate_positions_on_sky();
}

std::vector<vec3> Sky::generate_positions_on_sky() {

    std::vector<cgp::vec3> pos;
    for (int i = 0; i < N; i++)
    {
        float theta;
        float phi;
        float rand;
        bool test = true;
        vec3 p;
        while (test)
        {
            test = false;

            // Generating a random position on the sphere (uniform in the plane (phi, theta))
            theta = rand_interval(0, PI / 2);
            phi = rand_interval(0, 2 * PI);

            // For a uniform distribution on the sphere (according to the metric of the sphere)
            rand = rand_interval(0, 2 * PI);
            if (rand > 2 * PI * cos(theta)) {
                test = true;
                continue;
            }

            p = 0.9 * skyRadius * vec3{cos(theta)*cos(phi), cos(theta)*sin(phi), sin(theta)};

            float d_tree = 2 * 4 * 0.1f;

            // Rejecting stars too close from each other
            for (int j = 0; j < i; j++)
            {
                vec3 pj = pos[j];
                float d = norm(p - pj);
                if (d < 3 * starRadius)
                {
                    test = true;
                }
            }
        }

        pos.push_back(p);
    }

    return pos;
}

void Sky::display() {

    sky_sphere.transform.translation = environment->camera.position_camera;
    draw(sky_sphere, *environment);

    for (int k = 0; k < N; k++) {
        vec3 p = stars_positions[k];
        star_sphere.transform.translation = environment->camera.position_camera + p;
        draw(star_sphere, *environment);
    }

}