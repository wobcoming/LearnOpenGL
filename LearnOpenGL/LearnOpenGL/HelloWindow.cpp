#include <glad.h>
#include <GLFW\glfw3.h>
#include <iostream>


/**	we can register a callback function on the window that gets called each time the window is resized */
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main()
{
	/**
	* glfwInit
	* this function initializes the GLFW library. Before most GLFW functions can be used, GLFW must be initialized
	*/
	
	if (glfwInit())
	{
		/** 
		*	glfwWindowHint 
		*	this function tells us what option we want to configue,
		*	since the focus of this lesion is version 4.3, we'd like to tell GLFW that 4.3 is the version we want to use
		*/
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

		/** 
		*	Telling GLFW we want to use the core-profile means we'll get access to a smaller subset of OpenGL 
		*	features without backwards-compatible features we no longer need
		*/
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


		/** 
		*	create a window
		*/
		GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);

		if (window == nullptr)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return -1;
		}

		glfwMakeContextCurrent(window);
		
		/**
		*	GLAD manages function pointers for OpenGL so we want to initialize GLAD before we call any OpenGL function
		*/
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "failed to initialize glad" << std::endl;
			return -1;
		}

		/** 
		*	size of viewport
		*	The first two parameters of glViewport set the location of the lower left corner of the window. 
		*	The third and fourth parameter set the width and height of the rendering window in pixels, 
		*	which we set equal to GLFW's window size
		*/
		glViewport(0, 0, 800, 600);

		/** tell GLFW we want to call "framebuffer_size_callback" on every window resize by registering it */
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


		/** BEGIN RENDER LOOP */
		while (!glfwWindowShouldClose(window))
		{
			glfwSwapBuffers(window);

			/**
			*	The glfwPollEvents function checks if any events are triggered (like keyboard input or mouse movement events), 
			*	updates the window state, and calls the corresponding functions (which we can register via callback methods)
			*/
			glfwPollEvents();
		}

		/** As soon as we exit the render loop we would like to properly clean/delete all of GLFW's resources that were allocated */
		glfwTerminate();
		return 0;

	}
	

	return 0;
}