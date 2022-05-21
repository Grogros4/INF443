#include "tree.hpp"

using namespace cgp;

const double PI = 3.141592653589793238463;

mesh create_cylinder_mesh(float radius, float height)
{
    mesh m; 

    const int N = 20;
    // To do: fill this mesh ...
    // ...
    // To add a position: 
    //   m.position.push_back(vec3{x,y,z})
    // Or in pre-allocating the buffer:
    //   m.position.resize(maximalSize);
    //   m.position[index] = vec3{x,y,z}; (with 0<= index < maximalSize)
    // 
    // Similar with the triangle connectivity:
    //  m.connectivity.push_back(uint3{index_1, index_2, index_3});
    for (int k = 0; k < N + 1; k++) {
        m.position.push_back(vec3{ radius * cos( 2* PI * k / N ), radius * sin(2 * PI * k / N) ,0});
        m.position.push_back(vec3{ radius * cos(2 * PI * k / N), radius * sin(2 * PI * k / N) , height });
    }

    for (int k = 1; k < N; ++k)
    {
        m.connectivity.push_back(uint3{ 2 * k, 2 * k + 1, 2 * k + 2 });
        m.connectivity.push_back(uint3{ 2 * k, 2 * k - 1, 2 * k + 1 });
    }

    m.connectivity.push_back(uint3{ 0, 1, 2 });
    m.connectivity.push_back(uint3{ 2 * N,2 * N - 1,2 * N + 1 });
  
    

    // Need to call fill_empty_field() before returning the mesh 
    //  this function fill all empty buffer with default values (ex. normals, colors, etc).
    m.fill_empty_field();

    return m;
}

mesh create_cone_mesh(float radius, float height, float z_offset)
{
    mesh m; 
    // To do: fill this mesh ...
    // ...
    int N = 10;

    
    for (int k = 0; k < N; k++) {
        m.position.push_back(vec3{ radius * cos(2 * PI * k / N), radius * sin(2 * PI * k / N) ,z_offset });
    }
    m.position.push_back(vec3{ 0,0,height + z_offset });
    m.position.push_back(vec3{ 0,0,z_offset });
    
    for (int k = 0; k < N - 1; k++) {
        m.connectivity.push_back(uint3{k,N+1,k+1 });
        m.connectivity.push_back(uint3{ k,N,k+1 });
    }
    m.connectivity.push_back(uint3{ N-1,N + 1,0 });
    m.connectivity.push_back(uint3{ N-1,N, 0 });

    m.fill_empty_field();
    return m;
}

mesh create_tree()
{
    float h = 2; // trunk height
    float r = 0.5f; // trunk radius

    // Create a brown trunk
    mesh trunk = create_cylinder_mesh(r, h);
    trunk.color.fill({ 0.4f, 0.3f, 0.3f });


    // Create a green foliage from 3 cones
    mesh foliage = create_cone_mesh(4 * r, 6 * r, 0.0f);      // base-cone
    foliage.push_back(create_cone_mesh(4 * r, 6 * r, 2 * r));   // middle-cone
    foliage.push_back(create_cone_mesh(4 * r, 6 * r, 4 * r));   // top-cone
    foliage.position += vec3(0, 0, h);                 // place foliage at the top of the trunk
    foliage.color.fill({ 0.4f, 0.6f, 0.3f });

    // The tree is composted of the trunk and the foliage
    mesh tree = trunk;
    tree.push_back(foliage);

    return tree;
}


mesh create_mushroom()
{
    float h = 0.03f; // trunk height
    float r = 0.01f; // trunk radius

    // Create a brown trunk
    mesh trunk = create_cylinder_mesh(r, h);
    trunk.color.fill({ 0.9f, 0.9f, 0.9f });


    // Create a green foliage from 3 cones
    mesh foliage = create_cone_mesh(4 * r, 3 * r, 0.0f);      // base-cone
    foliage.position += vec3(0, 0, h);                 // place foliage at the top of the trunk
    foliage.color.fill({ 0.9f, 0.1f, 0.1f });

    // The tree is composted of the trunk and the foliage
    mesh tree = trunk;
    tree.push_back(foliage);

    return tree;
}
