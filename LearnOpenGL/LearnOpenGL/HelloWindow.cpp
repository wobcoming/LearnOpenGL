#include <glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include "3DObjectDefine.h"

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

/**	we can register a callback function on the window that gets called each time the window is resized */
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

/** input control in GLFW */
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

GLFWwindow* InitGLFW()
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
			return nullptr;
		}

		glfwMakeContextCurrent(window);

		/**
		*	GLAD manages function pointers for OpenGL so we want to initialize GLAD before we call any OpenGL function
		*/
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "failed to initialize glad" << std::endl;
			return nullptr;
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

		return window;
	}
	else
		return nullptr;
}

void isVertexShaderSuccessfullyCompile(GLuint vertexShader, GLenum status)
{
	int success;
	char infoLog[512];
	/**
	*	check if compilation was successful with glGetShaderiv. 
	*	If compilation failed, we should retrieve the error message with glGetShaderInfoLog and print the error message
	*/
	glGetShaderiv(vertexShader, status, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

void isProgramSuccessfullyLinked(GLuint shaderProgram, GLenum status)
{
	int success;
	char infoLog[512];
	/**
	*	check if compilation was successful with glGetShaderiv.
	*	If compilation failed, we should retrieve the error message with glGetShaderInfoLog and print the error message
	*/
	glGetProgramiv(shaderProgram, status, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

/** Create and assign shader */
unsigned int GetShaderProgram()
{
	// VERTEX SHADER
	/**
	*	provide the type of shader we want to create as an argument to glCreateShader.
	*	Since we're creating a vertex shader we pass in GL_VERTEX_SHADER.
	*/
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	/**
	*	Next we attach the shader source code to the shader object and compile the shader
	*/
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//	check if compilation was successful after the call to glCompileShader and if not checking for compile-time errors
	isVertexShaderSuccessfullyCompile(vertexShader, GL_COMPILE_STATUS);

	//	FRAGMENT SHADER
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	/**
	*	SHADER PROGRAM
	*
	*/
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//	check link error
	isProgramSuccessfullyLinked(shaderProgram, GL_LINK_STATUS);


	//delete the shader objects once we've linked them into the program object; we no longer need them anymore
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

/** create and assign buffer data */
void SetupBufferData(unsigned int &VAO, unsigned int &VBO, unsigned int &EBO)
{
	//	VERTEX ARRAY OBJECT
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	/**
	*	We manage this memory via so called vertex buffer objects (VBO) that can store a large number of vertices in the GPU's memory
	*	this buffer has a unique ID corresponding to that buffer
	*/
	glGenBuffers(1, &VBO);
	/**
	*	OpenGL has many types of buffer objects and the buffer type of a vertex buffer object is GL_ARRAY_BUFFER.
	*	OpenGL allows us to bind to several buffers at once as long as they have a different buffer type
	*/
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	/**
	*	any buffer calls we make (on the GL_ARRAY_BUFFER target) will be used to configure the currently bound buffer, which is VBO.
	*	Then we can make a call to the glBufferData function that copies the previously defined vertex data into the buffer's memory
	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(RecVertices), RecVertices, GL_STATIC_DRAW);

	// ELEMENT BUFFER OBJECT
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(RecIndices), RecIndices, GL_STATIC_DRAW);


	//	LINKING VERTEX ATTRIBUTES
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

int main()
{
		//init GLFW and create window
		GLFWwindow* window = InitGLFW();
		if (window == nullptr)
		{
			return -1;
		}

		unsigned int VAO, BVO, EBO;
		SetupBufferData(VAO, BVO, EBO);
		
		
		unsigned int shaderProgram = GetShaderProgram();



		/** BEGIN RENDER LOOP */
		while (!glfwWindowShouldClose(window))
		{
			/** check input	*/
			processInput(window);

			// clear color before render
			/**
			*	glClearColor sets the colorm value that OpenGL uses to reset the colorbuffer.
			*	As soon as glClear or glClearBuffer is called, it used the value from glClearColor to reset its color values
			*/
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			// the possible bit we cant set are GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT and GL_STENCIL_BUFFER_BIT
			glClear(GL_COLOR_BUFFER_BIT);

			//	ACTIVATE SHADER PROGRAM
			glUseProgram(shaderProgram);

			//	BIND VAO
			glBindVertexArray(VAO);

			// DRAW ARRAYS
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
			// -------------------------------------------------------------------------------
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