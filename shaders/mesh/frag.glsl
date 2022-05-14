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


//Exponential function to calculate fog intensity
float getFogFactor(float fogCoordinate)
{
	float result = 0.0;
	float density = 0.01;
	result = exp(-pow(density * fogCoordinate, 2.0));
	result = 1.0 - clamp(result, 0.0, 1.0);
	return result;
}


vec3 wavelength2rgb(float Wavelength)
{	
	float Gamma = 0.8f;
	float IntensityMax = 1.0f;
	float Red, Green, Blue;
	float factor;

	if((Wavelength >= 380) && (Wavelength < 440)) {
        Red = -(Wavelength - 440) / (440 - 380);
        Green = 0.0;
        Blue = 1.0;
    } else if((Wavelength >= 440) && (Wavelength < 490)) {
        Red = 0.0;
        Green = (Wavelength - 440) / (490 - 440);
        Blue = 1.0;
    } else if((Wavelength >= 490) && (Wavelength < 510)) {
        Red = 0.0;
        Green = 1.0;
        Blue = -(Wavelength - 510) / (510 - 490);
    } else if((Wavelength >= 510) && (Wavelength < 580)) {
        Red = (Wavelength - 510) / (580 - 510);
        Green = 1.0;
        Blue = 0.0;
    } else if((Wavelength >= 580) && (Wavelength < 645)) {
        Red = 1.0;
        Green = -(Wavelength - 645) / (645 - 580);
        Blue = 0.0;
    } else if((Wavelength >= 645) && (Wavelength < 781)) {
        Red = 1.0;
        Green = 0.0;
        Blue = 0.0;
    } else {
        Red = 0.0;
        Green = 0.0;
        Blue = 0.0;
    }

    // Let the intensity fall off near the vision limits

    if((Wavelength >= 380) && (Wavelength < 420)) {
        factor = 0.3 + 0.7 * (Wavelength - 380) / (420 - 380);
    } else if((Wavelength >= 420) && (Wavelength < 701)) {
        factor = 1.0;
    } else if((Wavelength >= 701) && (Wavelength < 781)) {
        factor = 0.3 + 0.7 * (780 - Wavelength) / (780 - 700);
    } else {
        factor = 0.0;
    }

    // Don't want 0^x = 1 for x <> 0
    Red = ( (Red == 0.0) ? 0 : IntensityMax * pow(Red * factor, Gamma) );
    Green = ( (Green == 0.0) ? 0 : IntensityMax * pow(Green * factor, Gamma) );
    Blue = ( (Blue == 0.0) ? 0 : IntensityMax * pow(Blue * factor, Gamma) );


	return vec3(Red, Green, Blue);
}

vec3 dopplerEffect(vec3 color)
{
	if (length(fragment.speed) > 0.0000001)
	{
		float ctheta = dot(fragment.speed, fragment.position)/(length(fragment.speed)*length(fragment.position));
		float b = length(fragment.speed) / c;
		float gamma = 1 / sqrt(1-b*b);
		float k = gamma * (1 + b * ctheta);
	
		float waveR = k * 645;
		float waveG = k * 510;
		float waveB = k * 440;

		vec3 colorR = color.x * wavelength2rgb(waveR);
		vec3 colorG = color.y * wavelength2rgb(waveG);
		vec3 colorB = color.z * wavelength2rgb(waveB);

		vec3 newColor;

		newColor.x = min(colorR.x + colorG.x + colorB.x, 1.0f);
		newColor.y = min(colorR.y + colorG.y + colorB.y, 1.0f);
		newColor.z = min(colorR.z + colorG.z + colorB.z, 1.0f);

		return newColor;
	}
	else
	{
		return color;
	}
	
}

vec3 dopplerEffect_new(vec3 color)
{
	if (length(fragment.speed) > 0.0000001)
	{
		float ctheta = dot(fragment.speed, fragment.position)/(length(fragment.speed)*length(fragment.position));
		float b = length(fragment.speed) / c;
		float gamma = 1 / sqrt(1-b*b);
		float dopp = b * gamma * ctheta;

		color = (gamma + dopp) * color; // Relativistic brightness

		vec3 shift;
		shift.r = 2 * max(0, 0.5 - abs(dopp + 0.0)) * color.r + 2 * max(0, 0.5 - abs(dopp + 0.5)) * color.g + 2 * max(0, 0.5 - abs(dopp + 1.0)) * color.b;
		shift.g = 2 * max(0, 0.5 - abs(dopp - 0.5)) * color.r + 2 * max(0, 0.5 - abs(dopp + 0.0)) * color.g + 2 * max(0, 0.5 - abs(dopp + 0.5)) * color.b;
		shift.b = 2 * max(0, 0.5 - abs(dopp - 1.0)) * color.r + 2 * max(0, 0.5 - abs(dopp - 0.5)) * color.g + 2 * max(0, 0.5 - abs(dopp + 0.0)) * color.b;

		return shift;
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
	vec3 camera_position = -O*last_col;

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
	vec4 p = view * model * vec4(fragment.position, 1.0);
	float fogCoordinate = abs(p.z);

	// Compute the base color of the object based on: vertex color, uniform color, and texture
	vec3 color_object  = fragment.color * color * color_image_texture.rgb;

	// Compute the final shaded color using Phong model
	vec3 color_shading = (Ka + Kd * diffuse) * color_object + Ks * specular * vec3(1.0, 1.0, 1.0);
	color_shading = dopplerEffect_new(color_shading);

	// Output color, with the alpha component
	//FragColor = mix(vec4(color_shading, alpha * color_image_texture.a), vec4(dopplerEffect_new(vec3(1.0,1.0,1.0)), 1.0),getFogFactor(fogCoordinate));
	FragColor = vec4(color_shading, 1);

}