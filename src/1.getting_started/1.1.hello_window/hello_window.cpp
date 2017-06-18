#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdlib.h>

void framebuffer_size_callback(GLFWwindow* window, int wight, wint height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIGHT = 800;
const unsigned int SCR_HEIGHT = 600;

using namespace std;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIGHT, SCR_HEIGHT, "Lear OpenGL", NULL, NULL);
    if (window == NULL)
    {
        cout << "window fail" << endl;
        exit(-1);
    }

    glfwMakeCurrentContext(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLloader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Fail to inti glad" << endl;
        exit(-1);
    }

    while(!glfwWindowShouldClose(window))
    {
        processInput(window);
        
        glfwSwapBuffers(window);
        glfwPullEvents();
    }


    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(true);
}

void framebuffer_size_callback(GLFWwindow* window, int wight, int height)
{
    glViewport(0, 0, wight, height);
}
