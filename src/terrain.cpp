
#include "terrain.hpp"


using namespace cgp;

// Evaluate 3D position of the terrain for any (u,v) \in [0,1]
float evaluate_terrain_height(float x, float y, float terrain_length)
{
    float u = x + 0.5*terrain_length;
    float v = y + 0.5*terrain_length;

    float hills = noise_perlin(0.05 * vec2{ u, v }, 1);
    float noise = 0.1 * noise_perlin({u, v}, 6, 0.35, 2);
    float z = hills + noise;

    return z;
}

float evaluate_hills_height(float x, float y, float terrain_length)
{
    float u = x + 0.5 * terrain_length;
    float v = y + 0.5 * terrain_length;

    float hills = noise_perlin(0.05 * vec2{ u, v }, 1);

    return hills;
}

mesh create_terrain_mesh(int N, float terrain_length, float scalex, float scaley)
{

    mesh terrain; // temporary terrain storage (CPU only)
    terrain.position.resize(N*N);
    terrain.uv.resize(N*N);

    // Fill terrain geometry
    for(int ku=0; ku<N; ++ku)
    {
        for(int kv=0; kv<N; ++kv)
        {
            // Compute local parametric coordinates (u,v) \in [0,1]
            float u = ku/(N-1.0f);
            float v = kv/(N-1.0f);

            // Compute the real coordinates (x,y) of the terrain 
            float x = (u - 0.5f) * terrain_length;
            float y = (v - 0.5f) * terrain_length;

            // Compute the surface height function at the given sampled coordinate
            cgp::vec2 p[4] = { cgp::vec2(-10,-10),cgp::vec2(5,5),cgp::vec2(-3,4),cgp::vec2(6,4) };
            float h[4] = { 3,-1.5,1,2 };
            float sigma[4] = { 10,3,4,4 };
            int L = 4;
            float z = evaluate_terrain_height(scalex * x, scaley * y, terrain_length);

            // Store vertex coordinates
            terrain.position[kv+N*ku] = {x,y,z};
            terrain.uv[kv+N*ku] = {u*10, v*10};
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

            terrain.connectivity.push_back(triangle_1);
            terrain.connectivity.push_back(triangle_2);
        }
    }

    // need to call this function to fill the other buffer with default values (normal, color, etc)
	terrain.fill_empty_field(); 

    return terrain;
}

std::vector<cgp::vec3> generate_positions_on_terrain(int N, float terrain_length)
{
    std::vector<cgp::vec3> pos;
    for (int i = 0; i < N; i++)
    {
        cgp::vec2 p[4] = { cgp::vec2(-10,-10),cgp::vec2(5,5),cgp::vec2(-3,4),cgp::vec2(6,4) };
        float h[4] = { 3,-1.5,1,2 };
        float sigma[4] = { 10,3,4,4 };
        int L = 4;

        float x;
        float y;
        bool test = true;
        while (test)
        {
            test = false;
            x = rand_interval(-0.5 * terrain_length, 0.5 * terrain_length);
            y = rand_interval(-0.5 * terrain_length, 0.5 * terrain_length);
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
        
        float z = evaluate_terrain_height(x, y, terrain_length);
        pos.push_back(vec3{ x,y,z });
    }

    return pos;
}