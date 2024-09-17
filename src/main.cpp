#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

static uint32_t width = 800;
static uint32_t height = 600;

void framebufferSizeCallback(GLFWwindow* pWindow, int width, int height);
void processInput(GLFWwindow* pWindow);

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

    std::cout << "Renderer: " << renderer << '\n';
    std::cout << "OpenGL version supported: " << version << '\n';

    // Register a callback function on the window that
    // gets called each time the window is resized.
    glfwSetFramebufferSizeCallback(pWindow, framebufferSizeCallback);

    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(pWindow))
    {
        processInput(pWindow);

        glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

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
