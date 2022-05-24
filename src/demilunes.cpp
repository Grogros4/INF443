#include "demilunes.hpp"

/** This file contains the custom implementation associated the scene 
*  The (*) indicates elements that are specific to the use of the camera mode and that should be handled if you want to use this effect in another scene. */

using namespace cgp;


void Demilunes::initialize(scene_environment_camera_head* env, Terrain* t)
{

	environment = env;
	terrain = t;

	mesh_drawable demilune_base;
	mesh_drawable demilune_saumon;
	mesh_drawable demilune_toit;
	mesh_drawable demilune_batiment;
	mesh_drawable demilune_briques;
	mesh_drawable demilune_blanc;
	mesh_drawable demilune_glass;
	mesh_drawable demilune_bandes;

	demilune_base.initialize(mesh_primitive_cube({ 0,-2, 0 }), "demilune_base");
	demilune_saumon.initialize(mesh_load_file_obj("assets/objects/demilune_saumon.obj"), "demilune_saumon");
	demilune_toit.initialize(mesh_load_file_obj("assets/objects/demilune_toit.obj"), "demilune_toit");
	demilune_batiment.initialize(mesh_load_file_obj("assets/objects/demilune_batiment.obj"), "demilune_batiment");
	demilune_briques.initialize(mesh_load_file_obj("assets/objects/demilune_briques.obj"), "demilune_briques");
	demilune_blanc.initialize(mesh_load_file_obj("assets/objects/demilune_blanc.obj"), "demilune_blanc");
	demilune_glass.initialize(mesh_load_file_obj("assets/objects/demilune_glass.obj"), "demilune_glass");
	demilune_bandes.initialize(mesh_load_file_obj("assets/objects/demilune_bandes.obj"), "demilune_bandes");

	demilune_saumon.shading.color = 2 * vec3{ 0.171441, 0.061246, 0.064803 };
	demilune_saumon.shading.phong = shading_parameters_phong::phong_parameters{1, 1, 0.5, 250};
	demilune_toit.shading.color = vec3{ 1, 1, 1 };
	demilune_toit.shading.phong = shading_parameters_phong::phong_parameters{ 1, 1, 0.5, 250 };
	demilune_batiment.shading.color = vec3{ 0.342383, 0.342383, 0.342383 };
	demilune_batiment.shading.phong = shading_parameters_phong::phong_parameters{ 1, 1, 0.5, 360 };
	demilune_briques.shading.color = vec3{ 0.076185, 0.035601, 0.039546 };
	demilune_briques.shading.phong = shading_parameters_phong::phong_parameters{ 1, 1, 0.5, 250 };
	demilune_blanc.shading.color = vec3{ 0.800000, 0.800000, 0.800000 };
	demilune_blanc.shading.phong = shading_parameters_phong::phong_parameters{ 1, 1, 0.5, 250 };
	demilune_glass.shading.color = vec3{ 0.200000, 0.200000, 0.200000 };
	demilune_glass.shading.phong = shading_parameters_phong::phong_parameters{ 1, 0, 0.5, 250 };
	demilune_bandes.shading.color = vec3{ 0.400000, 0.000000, 0.000000 };
	demilune_bandes.shading.phong = shading_parameters_phong::phong_parameters{ 1, 1, 0.5, 250 };

	demilune.add(demilune_base);
	demilune.add(demilune_saumon, "demilune_base");
	demilune.add(demilune_toit, "demilune_base");
	demilune.add(demilune_batiment, "demilune_base");
	demilune.add(demilune_briques, "demilune_base");
	demilune.add(demilune_blanc, "demilune_base");
	demilune.add(demilune_glass, "demilune_base");
	demilune.add(demilune_bandes, "demilune_base");
	
	mesh path_mesh = mesh_load_file_obj("assets/objects/path.obj");
	buffer<vec3> path_pos = path_mesh.position;
	for (int i = 0; i < path_pos.size(); i++)
	{
		vec3 p = path_pos[i];
		p.y += t->evaluate_hills_height(p.x, - p.z) + 0.1;
		path_pos[i] = p;
	}
	path_mesh.position = path_pos;

	path.initialize(path_mesh, "path");
	path.transform.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, Pi / 2.0f);
	path.transform.translation += vec3{ 0, 0, 0 };
	quaternion rot = path.transform.rotation.data;
	path.transform.rotation = rotation_transform::convert_axis_angle_to_quaternion({ 0,0,1 }, 0 * Pi / 180) * rot;
}

void Demilunes::display()
{

	demilune["demilune_base"].transform.rotation = rotation_transform::from_axis_angle({ 1,0,0 }, Pi / 2.0f);
	quaternion rot = demilune["demilune_base"].transform.rotation.data;
	demilune["demilune_base"].transform.translation = vec3{ 99.648, 3.73451, 0 };
	demilune["demilune_base"].transform.rotation = rotation_transform::convert_axis_angle_to_quaternion({ 0,0,1 }, -443 * Pi / 180) * rot;
	demilune.update_local_to_global_coordinates();
	draw(demilune, *environment);
	demilune["demilune_base"].transform.translation = vec3{ 69.6235, -18.8592, 0 };
	demilune["demilune_base"].transform.rotation = rotation_transform::convert_axis_angle_to_quaternion({ 0,0,1 }, -280 * Pi / 180) * rot;
	demilune.update_local_to_global_coordinates();
	draw(demilune, *environment);
	demilune["demilune_base"].transform.translation = vec3{ 86.8614, 94.0153, 0 };
	demilune["demilune_base"].transform.rotation = rotation_transform::convert_axis_angle_to_quaternion({ 0,0,1 }, -485 * Pi / 180) * rot;
	demilune.update_local_to_global_coordinates();
	draw(demilune, *environment);
	demilune["demilune_base"].transform.translation = vec3{ 55.8745, 92.1845, 0 };
	demilune["demilune_base"].transform.rotation = rotation_transform::convert_axis_angle_to_quaternion({ 0,0,1 }, -313 * Pi / 180) * rot;
	demilune.update_local_to_global_coordinates();
	draw(demilune, *environment);

	draw(path, *environment);

}