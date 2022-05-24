
#include "terrain.hpp"


using namespace cgp;

// Evaluate 3D position of the terrain for any (u,v) \in [0,1]
float Terrain::evaluate_hills_height(float x, float y)
{
    int scalex;
    int scaley;
    mat3 situation = get_mirroring(x, y);

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
    float u = x - get_matrix_coordinate(x) * chunk_size;
    float v = y - get_matrix_coordinate(y) * chunk_size;
    u = scalex * u;
    v = scaley * v;

    u = u + 0.5 * chunk_size;
    v = v + 0.5 * chunk_size;

    float hills = 10 * noise_perlin(0.05 * vec2{ u, v }, 1);

    return hills;
}

cgp::mesh Terrain::create_terrain_mesh(int N, float scalex, float scaley)
{

    mesh terrain_mesh; // temporary terrain storage (CPU only)
    terrain_mesh.position.resize(N*N);
    terrain_mesh.uv.resize(N*N);

    // Fill terrain geometry
    for(int ku=0; ku<N; ++ku)
    {
        for(int kv=0; kv<N; ++kv)
        {
            // Compute local parametric coordinates (u,v) \in [0,1]
            float u = ku/(N-1.0f);
            float v = kv/(N-1.0f);

            // Compute the real coordinates (x,y) of the terrain 
            float x = (u - 0.5f) * chunk_size;
            float y = (v - 0.5f) * chunk_size;

            // Compute the surface height function at the given sampled coordinate
            cgp::vec2 p[4] = { cgp::vec2(-10,-10),cgp::vec2(5,5),cgp::vec2(-3,4),cgp::vec2(6,4) };
            float h[4] = { 3,-1.5,1,2 };
            float sigma[4] = { 10,3,4,4 };
            int L = 4;
            float z = evaluate_hills_height(scalex * x, scaley * y);

            // Store vertex coordinates
            terrain_mesh.position[kv+N*ku] = {x,y,z};
            terrain_mesh.uv[kv+N*ku] = {u*10, v*10};
        }
    }

    // Generate triangle organization
    //  Parametric surface with uniform grid sampling: generate 2 triangles for each grid cell
    for(int ku=0; ku<N-1; ++ku)
    {
        for(int kv=0; kv<N-1; ++kv)
        {
            unsigned int idx = kv + N*ku; // current vertex offset

            uint3 triangle_1 = {idx, idx+1+N, idx+1};
            uint3 triangle_2 = {idx, idx+N, idx+1+N};

            terrain_mesh.connectivity.push_back(triangle_1);
            terrain_mesh.connectivity.push_back(triangle_2);
        }
    }

    // need to call this function to fill the other buffer with default values (normal, color, etc)
	terrain_mesh.fill_empty_field(); 

    return terrain_mesh;
}

void Terrain::initialize(scene_environment_camera_head* env, int N, float length)
{
    environment = env;
    chunk_size = length;

    mesh terrain_mesh = create_terrain_mesh(100, 1, 1);
    mesh terrain_meshx = create_terrain_mesh(100, -1, 1);
    mesh terrain_meshy = create_terrain_mesh(100, 1, -1);
    mesh terrain_meshxy = create_terrain_mesh(100, -1, -1);

    terrain.initialize(terrain_mesh, "Terrain");
    terrainx.initialize(terrain_meshx, "TerrainX");
    terrainy.initialize(terrain_meshy, "TerrainY");
    terrainxy.initialize(terrain_meshxy, "TerrainXY");

    terrain.shading.phong.specular = 0.0f;
    terrainx.shading.phong.specular = 0.0f;
    terrainy.shading.phong.specular = 0.0f;
    terrainxy.shading.phong.specular = 0.0f;
    GLuint const grass = opengl_load_texture_image("assets/texture_grass.jpg", GL_REPEAT, GL_REPEAT);
    terrain.texture = grass;
    terrainx.texture = grass;
    terrainy.texture = grass;
    terrainxy.texture = grass;

    tree_position = generate_positions_on_terrain(100, 1, 1);
    tree_positionx = generate_positions_on_terrain(100, -1, 1);
    tree_positiony = generate_positions_on_terrain(100, 1, -1);
    tree_positionxy = generate_positions_on_terrain(100, -1, -1);
}


int Terrain::get_matrix_coordinate(float x) {
    int l = chunk_size / 2;
    if (x + l < 0) {
        return(int(x - l) / chunk_size);
    }
    return(int(x + l) / chunk_size);
}

mat3 Terrain::get_mirroring(float x, float y) {
    mat3 m;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (get_matrix_coordinate(x - (i - 1) * chunk_size) % 2 == 0) {
                if (get_matrix_coordinate(y - (j - 1) * chunk_size) % 2 == 0) {
                    m[i][j] = 0;
                }
                else {
                    m[i][j] = 1;
                };
            }
            else {
                if (get_matrix_coordinate(y - (j - 1) * chunk_size) % 2 == 0) {
                    m[i][j] = 2;
                }
                else {
                    m[i][j] = 3;
                }
            };
        }
    }
    return(m);
}

void Terrain::display() {

    float x = environment->camera.position_camera.x;
    float y = environment->camera.position_camera.y;

    mat3 situation = get_mirroring(x, y);
    int u;
    int v;
    mesh const tree_mesh = trees.create_tree();
    mesh_drawable tree;
    tree.initialize(tree_mesh, "tree");

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            u = get_matrix_coordinate(x) + 1 - i;
            v = get_matrix_coordinate(y) + 1 - j;
            u = std::abs(u);
            v = std::abs(v);
            int a = situation[i][j];
            vec3 translation = { (2 - i - 1) * chunk_size + chunk_size * get_matrix_coordinate(x), (2 - j - 1) * chunk_size + chunk_size * get_matrix_coordinate(y), 0.0f };
            if (a == 0) {
                terrain.transform.translation = translation;
                draw(terrain, *environment);
                if ((u + v) >= 3) {
                    for (int k = 0; k < int(tree_position.size() * exp(-2 / sqrt((u + v) - 2))); k++) {
                        tree.transform.translation = tree_position[k] + translation;
                        draw(tree, *environment);
                    }
                }
            }
            if (a == 1) {
                terrainy.transform.translation = translation;
                draw(terrainy, *environment);
                if ((u + v) >= 3) {
                    for (int k = 0; k < int(tree_position.size() * exp(-2 / sqrt((u + v) - 2))); k++) {
                        tree.transform.translation = tree_positiony[k] + translation;
                        draw(tree, *environment);
                    }
                }
            }
            if (a == 2) {
                terrainx.transform.translation = translation;
                draw(terrainx, *environment);
                if ((u + v) >= 3) {
                    for (int k = 0; k < int(tree_position.size() * exp(-2 / sqrt((u + v) - 2))); k++) {
                        tree.transform.translation = tree_positionx[k] + translation;
                        draw(tree,* environment);
                    }
                }
            }
            if (a == 3) {
                terrainxy.transform.translation = translation;
                draw(terrainxy, *environment);
                if ((u + v) >= 3) {
                    for (int k = 0; k < int(tree_position.size() * exp(-2 / sqrt((u + v) - 2))); k++) {
                        tree.transform.translation = tree_positionxy[k] + translation;
                        draw(tree, *environment);
                    }
                }
            }
        }
    }

}std::vector<cgp::vec3> Terrain::generate_positions_on_terrain(int N, float scalex, float scaley)
{
    std::vector<cgp::vec3> pos;
    for (int i = 0; i < N; i++)
    {
        float x;
        float y;
        bool test = true;
        while (test)
        {
            test = false;
            x = rand_interval(-0.5 * chunk_size, 0.5 * chunk_size);
            y = rand_interval(-0.5 * chunk_size, 0.5 * chunk_size);
            vec2 v = vec2{ x,y };

            float d_tree = 2 * 4 * 0.1f;

            for (int j = 0; j < i; j++)
            {
                vec3 vt = pos[j];
                vec2 vv = vec2{ vt.x, vt.y };
                vec2 d = (v - vv);
                float dd = d.x * d.x + d.y * d.y;
                if (dd < d_tree * d_tree)
                {
                    test = true;
                }
            }
        }

        float z = evaluate_hills_height(scalex * x, scaley * y);
        pos.push_back(vec3{ x,y,z });
    }

    return pos;
}