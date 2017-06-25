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
Camera* camera = new Camera(glm::vec3(0.0f, 0.0f, 2.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

#define NUM_ARRAY_ELEMENTS(a) sizeof(a) / sizeof(*a)

const int COORDINATE_SCALAR = 10.0f;

// Coordinate system vertices
// ----------------------------------------------------
float coordinate_vertices[] = {
	// Positions
	0.5f,  0.0f,  0.0f,
	-0.5f,  0.0f,  0.0f,

	0.0f,  0.5f,  0.0f,
	0.0f, -0.5f,  0.0f,

	0.0f,  0.0f, -0.5f,
	0.0f,  0.0f,  0.5f
};

float coordinate_vertices_grid[] = {
	// Positions
	0.5f,  0.0f,  0.0f,
	-0.5f,  0.0f,  0.0f
};

// ----------------------------------------------------

// cube vertices
// ----------------------------------------------------
float cube_vertices[] =
{
	-0.5f, +0.5f, +0.5f,  // 0
	+0.0f, +0.0f, +1.0f,	// Color
	+0.5f, +0.5f, +0.5f,  // 1
	+0.0f, +0.0f, +1.0f,	// Color
	+0.5f, +0.5f, -0.5f,  // 2
	+0.0f, +0.0f, +1.0f,  // Color
	-0.5f, +0.5f, -0.5f,  // 3
	+0.0f, +0.0f, +1.0f,  // Color

	-0.5f, +0.5f, -0.5f,  // 4
	+0.0f, +0.0f, +1.0f,  // Color
	+0.5f, +0.5f, -0.5f,  // 5
	+0.0f, +0.0f, +1.0f,  // Color
	+0.5f, -0.5f, -0.5f,  // 6
	+0.0f, +0.0f, +1.0f,  // Color
	-0.5f, -0.5f, -0.5f,  // 7
	+0.0f, +0.0f, +1.0f,  // Color

	+0.5f, +0.5f, -0.5f,  // 8
	+0.0f, +0.0f, +1.0f,  // Color
	+0.5f, +0.5f, +0.5f,  // 9
	+0.0f, +0.0f, +1.0f,  // Color
	+0.5f, -0.5f, +0.5f,  // 10
	+0.0f, +0.0f, +1.0f,  // Color
	+0.5f, -0.5f, -0.5f,  // 11
	+0.0f, +0.0f, +1.0f,  // Color

	-0.5f, +0.5f, +0.5f,  // 12
	+0.0f, +0.0f, +1.0f,  // Color
	-0.5f, +0.5f, -0.5f,  // 13
	+0.0f, +0.0f, +1.0f,  // Color
	-0.5f, -0.5f, -0.5f,  // 14
	+0.0f, +0.0f, +1.0f,  // Color
	-0.5f, -0.5f, +0.5f,  // 15
	+0.0f, +0.0f, +1.0f,  // Color

	+0.5f, +0.5f, +0.5f,  // 16
	+1.0f, +1.0f, +0.0f,  // Color
	-0.5f, +0.5f, +0.5f,  // 17
	+1.0f, +1.0f, +0.0f,  // Color
	-0.5f, -0.5f, +0.5f,  // 18
	+1.0f, +1.0f, +0.0f,  // Color
	+0.5f, -0.5f, +0.5f,  // 19
	+1.0f, +1.0f, +0.0f,  // Color

	+0.5f, -0.5f, -0.5f,  // 20
	+0.0f, +0.0f, +1.0f,  // Color
	-0.5f, -0.5f, -0.5f,  // 21
	+0.0f, +0.0f, +1.0f,  // Color
	-0.5f, -0.5f, +0.5f,  // 22
	+0.0f, +0.0f, +1.0f,  // Color
	+0.5f, -0.5f, +0.5f,  // 23
	+0.0f, +0.0f, +1.0f,  // Color
};

unsigned short cube_indices[] = {
	0,   1,  2,  0,  2,  3, // Top
	4,   5,  6,  4,  6,  7, // Front
	8,   9, 10,  8, 10, 11, // Right
	12, 13, 14, 12, 14, 15, // Left
	16, 17, 18, 16, 18, 19, // Back
	20, 22, 21, 20, 23, 22, // Bottom
};

//


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
	Shader coordinatesShader("0.1.coordinates.vs", "0.1.coordinates.fs");
	Shader cubeShader("0.1.cube.vs", "0.1.cube.fs");


	// Coordinate system
	// ----------------------------------------------------
	unsigned int coordinateVAO, coordinateVBO;
	glGenVertexArrays(1, &coordinateVAO);
	glGenBuffers(1, &coordinateVBO);
	glBindVertexArray(coordinateVAO);
	glBindBuffer(GL_ARRAY_BUFFER, coordinateVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(coordinate_vertices), &coordinate_vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// ----------------------------------------------------

	// Coordinate system grid
	// ----------------------------------------------------
	unsigned int coordinate_grid_VAO, coordinate_grid_VBO;
	glGenVertexArrays(1, &coordinate_grid_VAO);
	glGenBuffers(1, &coordinate_grid_VBO);
	glBindVertexArray(coordinate_grid_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, coordinate_grid_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(coordinate_vertices_grid), &coordinate_vertices_grid, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// ----------------------------------------------------

	// Cube
	// ----------------------------------------------------
	unsigned int cubeVAO, cubeVBO, cubeEBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glGenBuffers(1, &cubeEBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), &cube_vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), &cube_indices, GL_STATIC_DRAW);	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
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
		coordinatesShader.use();
		glm::mat4 model;
		glm::mat4 view = camera->GetViewMatrix();
		glm::mat4 projection = glm::perspective(camera->Zoom, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		coordinatesShader.setMat4("model", model);
		coordinatesShader.setMat4("view", view);
		coordinatesShader.setMat4("projection", projection);
		// Draw main coordinate lines
		coordinatesShader.setVec3("lineColor", glm::vec3(1.0f));
		glBindVertexArray(coordinateVAO);
		glDrawArrays(GL_LINES, 0, 6);
		glBindVertexArray(0);

		// Draw coordinate grid X
		glBindVertexArray(coordinate_grid_VAO);
		coordinatesShader.setVec3("lineColor", glm::vec3(1.0f, 0.0f, 0.0f));
		for(int i = -5; i <= 5; i++)
		{
			glm::mat4 model;
			model = glm::translate(model, glm::vec3(0.0f,  0.0f, (float)i));
			model = glm::scale(model, glm::vec3(COORDINATE_SCALAR)); 
			coordinatesShader.setMat4("model", model);
			glDrawArrays(GL_LINES, 0, 2);

			model = glm::mat4();
			model = glm::translate(model, glm::vec3((float)i,  0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(COORDINATE_SCALAR));
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			coordinatesShader.setMat4("model", model);
			glDrawArrays(GL_LINES, 0, 2);
		}
		glBindVertexArray(0);

		// Draw first cube
		cubeShader.use();
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(-2.0f, 0.0f, -2.0f));
		cubeShader.setMat4("model", model);
		cubeShader.setMat4("view", view);
		cubeShader.setMat4("projection", projection);

		glBindVertexArray(cubeVAO);
		glDrawElements(GL_TRIANGLES, NUM_ARRAY_ELEMENTS(cube_indices), GL_UNSIGNED_SHORT, 0);
		glBindVertexArray(0);


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &coordinateVAO);
	glDeleteBuffers(1, &coordinateVBO);
	glDeleteVertexArrays(1, &coordinate_grid_VAO);
	glDeleteBuffers(1, &coordinate_grid_VBO);

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &cubeEBO);

	glfwTerminate();

	return 0;
}