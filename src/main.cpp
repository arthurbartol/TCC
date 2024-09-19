#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "util/Util.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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

#define NUM_VAOs 1
#define NUM_VBOs 2

GLuint vaos[NUM_VAOs];
GLuint vbos[NUM_VBOs];
GLuint renderingProgram;

float cameraX, cameraY, cameraZ;
float cubeLocationX, cubeLocationY, cubeLocationZ;

GLuint mvLocation, pLocation;
int width, height;
float aspectRatio;
glm::mat4 pMatrix, vMatrix, mMatrix, mvMatrix, tMatrix, rMatrix;

void setupVertices()
{
    // 36 vertices,12 triangles, makes 2x2x2 cube placed at origin
    float vertexPositions[108] = {
        -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f, -1.0f,
         1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f
    };

    glGenVertexArrays(NUM_VAOs, vaos);
    glBindVertexArray(vaos[0]);

    glGenBuffers(NUM_VBOs, vbos);
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
}

void init(GLFWwindow* pWindow)
{
    renderingProgram = Util::createShaderProgram("../src/shaders/vertex.glsl", "../src/shaders/fragment.glsl");

    cameraX = 0.0f;
    cameraY = 0.0f;
    cameraZ = 8.0f;

    cubeLocationX =  0.0f;  //
    cubeLocationY = -2.0f;  // shift down Y to reveal perspective
    cubeLocationZ =  0.0f;  //

    setupVertices();
}

void display(GLFWwindow* pWindow, double currentTime)
{
    glClear(GL_DEPTH_BUFFER_BIT);
    // glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(renderingProgram);

    // Get the uniform variables for the model-view and projection matrices
    pLocation = glGetUniformLocation(renderingProgram, "pMatrix");
    mvLocation = glGetUniformLocation(renderingProgram, "mvMatrix");

    // Build perspective matrix
    glfwGetFramebufferSize(pWindow, &width, &height);
    aspectRatio = (float)width / (float)height;
    pMatrix = glm::perspective(1.0472f, aspectRatio, 0.1f, 1000.0f);    // 1.0472 radians = 60 degrees

    // Use current time to compute different translations in x, y and z
    tMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(std::sin(0.35f * currentTime) * 2.0f,
                                                        std::cos(0.52f * currentTime) * 2.0f,
                                                        std::sin(0.70f * currentTime) * 2.0f));
    rMatrix = glm::rotate(glm::mat4(1.0f), 1.75f * (float)currentTime, glm::vec3(0.0f, 1.0f, 0.0f));    //
    rMatrix = glm::rotate(rMatrix, 1.75f * (float)currentTime, glm::vec3(1.0f, 0.0f, 0.0f));            // the 1.75 adjusts the rotation speed
    rMatrix = glm::rotate(rMatrix, 1.75f * (float)currentTime, glm::vec3(0.0f, 0.0f, 1.0f));            //

    mMatrix = tMatrix * rMatrix;

    // Build view, model and model-view matrix
    vMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
    // mMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(cubeLocationX, cubeLocationY, cubeLocationZ));
    mvMatrix = vMatrix * mMatrix;

    // Copy perspective and model-view matrices to corresponding uniform variables
    glUniformMatrix4fv(pLocation, 1, GL_FALSE, glm::value_ptr(pMatrix));
    glUniformMatrix4fv(mvLocation, 1, GL_FALSE, glm::value_ptr(mvMatrix));

    // Associate VBO with the corresponding vertex attribute in the vertex shader
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // Adjust OpenGL settings and draw model
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

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

    GLFWwindow* pWindow = glfwCreateWindow(600, 600, "TCC", nullptr, nullptr);
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
