#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "include/Utils.h"

#include <iostream>
#include <fstream>
#include <string>

static uint32_t width = 800;
static uint32_t height = 600;

void framebufferSizeCallback(GLFWwindow* pWindow, int width, int height);
void processInput(GLFWwindow* pWindow);

#define NUM_VAOs 1

GLuint vaos[NUM_VAOs];
GLuint renderingProgram;

void init(GLFWwindow* pWindow);

static float x = 0.0f;
static float inc = 0.01f;

void display(GLFWwindow* pWindow, double currentTime);

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* pWindow = glfwCreateWindow(width, height, "TCC", nullptr, nullptr);
    if (!pWindow)
    {
        const char* description = nullptr;
        int code = glfwGetError(&description);

        std::cerr << "Failed to create a GLFW window" << std::endl;

        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(pWindow);

    GLenum glewStatus = glewInit();
    if (glewStatus != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(glewStatus) << std::endl;
        
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Check OpenGL version and renderer.
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);

    std::clog << "Renderer: " << renderer << std::endl;
    std::clog << "OpenGL version supported: " << version << std::endl;
    std::clog << std::endl;

    // Register a callback function on the window that
    // gets called each time the window is resized.
    glfwSetFramebufferSizeCallback(pWindow, framebufferSizeCallback);

    glfwSwapInterval(1);

    init(pWindow);

    while (!glfwWindowShouldClose(pWindow))
    {
        processInput(pWindow);

        display(pWindow, glfwGetTime());

        glfwSwapBuffers(pWindow);
        glfwPollEvents();
    }

    glfwDestroyWindow(pWindow);
    
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

void framebufferSizeCallback(GLFWwindow* pWindow, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* pWindow)
{
    if (glfwGetKey(pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, true);
    }
}

void init(GLFWwindow* pWindow)
{
    renderingProgram = Utils::createShaderProgram("shaders/vertex.glsl", "shaders/fragment.glsl");

    glGenVertexArrays(NUM_VAOs, vaos);
    glBindVertexArray(vaos[0]);
}

void display(GLFWwindow* window, double currentTime)
{
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(renderingProgram);

    x += inc;
    if (x > 1.0f) inc = -0.01f;
    if (x < -1.0f) inc = 0.01f;

    GLuint offsetLocation = glGetUniformLocation(renderingProgram, "offset");   // get ptr to "offset"
    glProgramUniform1f(renderingProgram, offsetLocation, x);                    // send value "x" to "offset"

    glDrawArrays(GL_TRIANGLES, 0, 3);
}
