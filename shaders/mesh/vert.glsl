#version 330 core // OpenGL 3.3 shader

// Vertex shader - this code is executed for every vertex of the shape

// Inputs coming from VBOs
layout (location = 0) in vec3 position;  // vertex position in local space (x,y,z)
layout (location = 1) in vec3 normal;    // vertex normal in local space   (nx,ny,nz)
layout (location = 2) in vec3 color;     // vertex color      (r,g,b)
layout (location = 3) in vec2 uv;        // vertex uv-texture (u,v)


// Output variables sent to the fragment shader
out struct fragment_data
{
    vec3 position; // vertex position in world space
    vec3 normal;   // normal position in world space
    vec3 color;    // vertex color
    vec2 uv;       // vertex uv
	vec3 speed;
} fragment;

// Uniform variables expected to receive from the C++ program
uniform mat4 model;      // Model affine transform matrix associated to the current shape
uniform mat4 view;       // View matrix (rigid transform) of the camera
uniform mat4 projection; // Projection (perspective or orthogonal) matrix of the camera
uniform vec3 speed;      // Player speed
uniform float c;   		 // Speed of light

void main()
{

    // The position of the vertex in the world space relative to camera
	vec4 p = view * model * vec4(position, 1.0);
	p = p / p.w;
    float norme = length(p.xyz);
	vec4 speed4 = vec4(speed,0);
	speed4 = view * speed4;

	//Calcul relativiste
	if (length(speed) > 0.00000001){
		float b = length(speed)/c;
		//b = 0;
		float gamma = 1/sqrt(1-b*b);

		//Calcul du repère orthogonal vers v
		vec3 uve = speed4.xyz / length(speed4);
		vec3 utheta = p.xyz - dot(p.xyz, uve)*uve;
		utheta = utheta/length(utheta);
		vec3 uz = cross(uve,utheta);
	




		float ctheta = dot(speed4, p)/(length(speed4)*length(p.xyz));
		float stheta = dot(uz,cross(speed4.xyz, p.xyz))/(length(speed4)*length(p.xyz));

		float cosi = (ctheta + b)/(1 + b*ctheta);
		float sini = stheta / (gamma*(1 + b*ctheta));

		

		p.xyz = norme*(cosi * uve + sini * utheta);
		}




	// The normal of the vertex in the world space
	vec4 n = model * vec4(normal, 0.0);

	// The projected position of the vertex in the normalized device coordinates:
	vec4 p_proj = projection * p;


	// Fill the parameters sent to the fragment shader
	fragment.position = p.xyz;
	fragment.normal   = n.xyz;
	fragment.color = color;
	fragment.uv = uv;
	fragment.speed = speed4.xyz;

	// gl_Position is a built-in variable which is the expected output of the vertex shader
	gl_Position = p_proj; // gl_Position is the projected vertex position (in normalized device coordinates)
}
