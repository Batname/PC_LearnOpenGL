#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>
#include <functions.h>

#include <iostream>
using namespace std;

unsigned int loadCubemap(vector<string> faces);

const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 720;

GLFWwindow* window;
Camera* camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

#define NUM_ARRAY_ELEMENTS(a) sizeof(a) / sizeof(*a)

// Coordinate system vertices
// ----------------------------------------------------
float coordinate_vertices[] = {
	// Positions         // Colors
	0.5f,  0.0f,  0.0f,   1.0f, 1.0f, 1.0f,  // X main
	-0.5f,  0.0f,  0.0f,   1.0f, 1.0f, 1.0f,  // X main
	0.5f,  0.0f,  0.25f,   1.0f, 0.0f, 0.0f, // X 1l
	-0.5f,  0.0f,  0.25f,   1.0f, 0.0f, 0.0f, // X 1l
	0.5f,  0.0f, -0.25f,   1.0f, 0.0f, 0.0f, // X 1r
	-0.5f,  0.0f, -0.25f,   1.0f, 0.0f, 0.0f, // X 1r

	0.5f,  0.0f,  0.5f,   1.0f, 0.0f, 0.0f,  // X 2l
	-0.5f,  0.0f,  0.5f,   1.0f, 0.0f, 0.0f,  // X 2l
	0.5f,  0.0f, -0.5f,   1.0f, 0.0f, 0.0f,  // X 2r
	-0.5f,  0.0f, -0.5f,   1.0f, 0.0f, 0.0f,  // X 2r

	0.0f,  0.5f,  0.0f,   1.0f, 1.0f, 1.0f,  // Y main
	0.0f, -0.5f,  0.0f,   1.0f, 1.0f, 1.0f,  // Y main

	0.0f,  0.0f, -0.5f,   1.0f, 1.0f, 1.0f,  // Z main
	0.0f,  0.0f,  0.5f,   1.0f, 1.0f, 1.0f,  // Z main
	0.25f,  0.0f, -0.5f,   1.0f, 0.0f, 0.0f, // Z 1l
	0.25f,  0.0f,  0.5f,   1.0f, 0.0f, 0.0f, // Z 1l
	-0.25f,  0.0f, -0.5f,   1.0f, 0.0f, 0.0f, // Z 1r
	-0.25f,  0.0f,  0.5f,   1.0f, 0.0f, 0.0f, // Z 1r

	0.5f,  0.0f, -0.5f,   1.0f, 0.0f, 0.0f,  // Z 2l
	0.5f,  0.0f,  0.5f,   1.0f, 0.0f, 0.0f,  // Z 2l
	-0.5f,  0.0f, -0.5f,   1.0f, 0.0f, 0.0f,  // Z 2r
	-0.5f,  0.0f,  0.5f,   1.0f, 0.0f, 0.0f   // Z 2r
};

// ----------------------------------------------------


int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
	    cout << "Failed to create GLFW window" << endl;
	    glfwTerminate();
	    return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
	    cout << "Failed to initialize GLAD" << endl;
	    return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// Basic Shader
	Shader basicShader("0.1.basic.vs", "0.1.basic.fs");


	// Coordinate system
	// ----------------------------------------------------
	unsigned int coordinateVAO, coordinateVBO;
	glGenVertexArrays(1, &coordinateVAO);
	glGenBuffers(1, &coordinateVBO);
	glBindVertexArray(coordinateVAO);
	glBindBuffer(GL_ARRAY_BUFFER, coordinateVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(coordinate_vertices), &coordinate_vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));

	// ----------------------------------------------------



	while(!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// draw scene as normal
		basicShader.use();
		glm::mat4 model;
		glm::mat4 view = camera->GetViewMatrix();
		glm::mat4 projection = glm::perspective(camera->Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		basicShader.setMat4("model", model);
		basicShader.setMat4("view", view);
		basicShader.setMat4("projection", projection);
		// draw coordinates
		glBindVertexArray(coordinateVAO);
		glDrawArrays(GL_LINES, 0, 22);
		glBindVertexArray(0);


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &coordinateVAO);
	glDeleteBuffers(1, &coordinateVBO);

	glfwTerminate();

	return 0;
}