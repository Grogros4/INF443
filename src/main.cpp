
#include "cgp/cgp.hpp" // Give access to the complete CGP library
#include <iostream> 

// Custom scene of this code
#include "scene.hpp"
#include "player_mover.hpp"


// *************************** //
// Global Variables		
// *************************** //

// A helper tool to factorize common default functions (Window initialization, code that starts frame, etc)
cgp::helper_common_scene helper_common;


// The custom structure of the current scene defined in "scene.hpp"
scene_structure scene;
PlayerMover playerMover;




// *************************** //
// Start of the program
// *************************** //

GLFWwindow* standard_window_initialization(int width = 0, int height = 0);

int main(int, char* argv[])
{
	std::cout << "Run " << argv[0] << std::endl;


	// ************************ //
	//     INITIALISATION
	// ************************ //
	
	// Standard Initialization with dimension in pixels
	GLFWwindow* window = standard_window_initialization(2048, 1096); 

	// Custom scene initialization
	std::cout << "Initialize data of the scene ..." << std::endl;
	scene.initialize();
	playerMover.initialize(&scene, &window);
	std::cout << "Initialization success" << std::endl;
	

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	// ************************ //
	//     Animation Loop
	// ************************ //
	std::cout<<"Start animation loop ..."<<std::endl;
	while (!glfwWindowShouldClose(window))
	{
		// Reset the screen for a new frame
		helper_common.frame_begin(scene.environment.background_color, window, scene.inputs.window, scene.inputs.mouse.on_gui);
		scene.environment.projection.update_aspect_ratio(scene.inputs.window.aspect_ratio());
		scene.environment_hud.projection.update_aspect_ratio(scene.inputs.window.aspect_ratio());

		// Update the camera position at every frame for a fly-mode

		//scene.update_camera(mouse_x, mouse_y);
		playerMover.update_camera();

		// Call the display of the scene
		scene.display();
		
		// End of ImGui display and handle GLFW events
		helper_common.frame_end(window);
	}
	
	// Cleanup
	cgp::imgui_cleanup();
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}



// This function is called everytime the window is resized
void window_size_callback(GLFWwindow* , int width, int height)
{
	scene.inputs.window = { width, height };
}

// This function is called everytime the mouse is moved
void mouse_move_callback(GLFWwindow* /*window*/, double xpos, double ypos)
{
	scene.inputs.mouse_position_update( { xpos, ypos } );
	playerMover.update_mouse(xpos, ypos);

	// Remove the standard trackball behavior in the main function (*)
	//   The motion is now computed in the animation loop at each frame
}

// This function is called everytime a mouse button is clicked/released
void mouse_click_callback(GLFWwindow* /*window*/, int button, int action, int /*mods*/)
{
	scene.inputs.mouse.click.update_from_glfw_click(button, action);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	playerMover.update_c(yoffset);
}


// This function is called everytime a keyboard touch is pressed/released
void keyboard_callback(GLFWwindow* /*window*/, int key, int , int action, int /*mods*/)
{
	scene.inputs.keyboard.update_from_glfw_key(key, action);
	if (key == GLFW_KEY_SPACE)
	{
		if (action == GLFW_PRESS)
			playerMover.isJumping = true;
		else if (action == GLFW_RELEASE)
			playerMover.isJumping = false;
	}
	if (key == GLFW_KEY_DOWN)
	{
		if (action == GLFW_PRESS)
			playerMover.isCrouching = true;
		else if (action == GLFW_RELEASE)
			playerMover.isCrouching = false;
	}
	if (key == GLFW_KEY_W)
	{
		if (action == GLFW_PRESS)
			playerMover.isMovingForward = true;
		else if (action == GLFW_RELEASE)
			playerMover.isMovingForward = false;
	}
	if (key == GLFW_KEY_S)
	{
		if (action == GLFW_PRESS)
			playerMover.isMovingBackward = true;
		else if (action == GLFW_RELEASE)
			playerMover.isMovingBackward = false;
	}
	if (key == GLFW_KEY_A)
	{
		if (action == GLFW_PRESS)
			playerMover.isMovingLeftward = true;
		else if (action == GLFW_RELEASE)
			playerMover.isMovingLeftward = false;
	}
	if (key == GLFW_KEY_D)
	{
		if (action == GLFW_PRESS)
			playerMover.isMovingRightward = true;
		else if (action == GLFW_RELEASE)
			playerMover.isMovingRightward = false;
	}
	if (key == GLFW_KEY_ESCAPE)
	{
		if (action == GLFW_PRESS)
		{
			playerMover.swapMenu();
		}
	}
}

// Standard initialization procedure
GLFWwindow* standard_window_initialization(int width_target, int height_target)
{
	// Create the window using GLFW
	GLFWwindow* window = cgp::create_window(width_target, height_target);
	
	// Update storage for window size for the scene
	int width = 0, height = 0;
	glfwGetWindowSize(window, &width, &height);
	scene.inputs.window = { width, height };

	// Display debug information on command line
	std::cout << cgp::opengl_info_display() << std::endl;

	// Initialize ImGUI
	cgp::imgui_init(window);

	// Set the callback functions for the inputs
	glfwSetKeyCallback(window, keyboard_callback);            // Event called when a keyboard touch is pressed/released
	glfwSetMouseButtonCallback(window, mouse_click_callback); // Event called when a button of the mouse is clicked/released
	glfwSetCursorPosCallback(window, mouse_move_callback);    // Event called when the mouse is moved
	glfwSetWindowSizeCallback(window, window_size_callback);  // Event called when the window is rescaled
	glfwSetScrollCallback(window, scroll_callback);

	// Load default shader and initialize default frame
	helper_common.initialize();

	return window;
}