#version 330 core  // OpenGL 3.3 shader

// Fragment shader - this code is executed for every pixel/fragment that belongs to a displayed shape

// Inputs coming from tehe vertex shader
in struct fragment_data
{ 
    vec3 position; // position in the world space
    vec3 normal;   // normal in the world space
    vec3 color;    // current color on the fragment
    vec2 uv;       // current uv-texture on the fragment
	vec3 speed;
} fragment;

// Output of the fragment shader - output color
layout(location=0) out vec4 FragColor;

// Texture image
uniform sampler2D image_texture;

// Uniform values - must be send from the C++ code
uniform vec3 light; // position of the light

// Shape color
uniform vec3 color;   // Uniform color of the object
uniform float alpha;  // alpha coefficient

// Phong shading
uniform float Ka;     // Ambient coefficient
uniform float Kd;     // Diffuse coefficient
uniform float Ks;     // Specular coefficient
uniform float specular_exp; // Specular exponent

uniform mat4 model;
uniform mat4 view;       // View matrix (rigid transform) of the camera - to compute the camera position

uniform float c;

uniform bool doppler_effect;
uniform bool relativistic_brightness;

vec3 camera_position;


//Exponential function to calculate fog intensity
float getFogFactor(vec3 pp)
{
	float result = 0.0;
	float density = 0.01;
	float z = 100;
	float fogCoordinate = length(pp.xy) * exp(-pp.z / z);
	result = exp(-pow(density * fogCoordinate, 2.0));
	result = 1.0 - clamp(result, 0.0, 1.0);
	return result;
}

vec3 dopplerEffect(vec3 color)
{
	if (length(fragment.speed) > 0.0000001)
	{
		vec3 pos = fragment.position - camera_position;

		// Doppler effect formula
		float ctheta = dot(fragment.speed, pos)/(length(fragment.speed)*length(pos));
		float b = length(fragment.speed) / c;
		float gamma = 1 / sqrt(1-b*b);
		float dopp = b * gamma * ctheta;

		// Relativistic brightness
		if (relativistic_brightness)
			color = (gamma + dopp) * color;

		// Rectangle shift
		vec3 shift;
		shift.r = 2 * max(0, 0.5 - abs(dopp + 0.0)) * color.r + 2 * max(0, 0.5 - abs(dopp + 0.5)) * color.g + 2 * max(0, 0.5 - abs(dopp + 1.0)) * color.b;
		shift.g = 2 * max(0, 0.5 - abs(dopp - 0.5)) * color.r + 2 * max(0, 0.5 - abs(dopp + 0.0)) * color.g + 2 * max(0, 0.5 - abs(dopp + 0.5)) * color.b;
		shift.b = 2 * max(0, 0.5 - abs(dopp - 1.0)) * color.r + 2 * max(0, 0.5 - abs(dopp - 0.5)) * color.g + 2 * max(0, 0.5 - abs(dopp + 0.0)) * color.b;

		if (doppler_effect)
			color = shift;

		return color;

	}
	else
	{
		return color;
	}
}

void main()
{

	// Compute the position of the center of the camera
	mat3 O = transpose(mat3(view));                   // get the orientation matrix
	vec3 last_col = vec3(view*vec4(0.0, 0.0, 0.0, 1.0)); // get the last column
	camera_position = -O*last_col;

	// Re-normalize the normals (interpolated on the triangle)
	vec3 N = normalize(fragment.normal);

	// Inverse the normal if it is viewed from its back
	//  (note: doesn't work on Mac)
	if (gl_FrontFacing == false) {  
		N = -N;
	}

	// Phong coefficient (diffuse, specular)
	// ************************* //

	// Unit direction toward the light
	vec3 L = normalize(light-fragment.position);

	// Diffuse coefficient
	float diffuse = max(dot(N,L),0.0);

	// Specular coefficient
	float specular = 0.0;
	if(diffuse>0.0){
		vec3 R = reflect(-L,N); // symetric of light-direction with respect to the normal
		vec3 V = normalize(camera_position-fragment.position);
		specular = pow( max(dot(R,V),0.0), specular_exp );
	}

	// Texture
	// ************************* //
	
	// Current uv coordinates
	// by default inverse the v direction (avoids common image upside-down)
	vec2 uv_image = vec2(fragment.uv.x, 1.0-fragment.uv.y);

	// Get the current texture color
	vec4 color_image_texture = texture(image_texture, uv_image);


	// Compute Shading with fog
	// ************************* //

	//Get the position of the object relative to the camera to apply fog accordingly
	
	vec3 p = fragment.position; 
	p -= camera_position;
	float fogCoordinate = max(sqrt(p.x*p.x + p.y*p.y), 0);
	vec4 fogColor = vec4(0,0,0.1,0);
	float fogFactor = getFogFactor(p);

	// Compute the base color of the object based on: vertex color, uniform color, and texture
	vec3 color_object  = fragment.color * color * color_image_texture.rgb;

	// Compute the final shaded color using Phong model
	vec3 color_shading = (Ka + Kd * diffuse) * color_object + Ks * specular * vec3(1.0, 1.0, 1.0);

	// Output color, with the alpha component
	FragColor = vec4(color_shading, alpha * color_image_texture.a);
	FragColor = mix(FragColor, fogColor, fogFactor);
	FragColor.xyz = dopplerEffect(FragColor.xyz);

}