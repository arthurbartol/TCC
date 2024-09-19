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

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

#define NUM_VAOs 1
#define NUM_VBOs 2

GLuint vaos[NUM_VAOs];
GLuint vbos[NUM_VBOs];
GLuint renderingProgram;

float cameraX, cameraY, cameraZ;

GLuint vLocation, pLocation, tfLocation;
int width, height;
float aspectRatio;
glm::mat4 pMatrix, vMatrix, mMatrix, mvMatrix, tMatrix, rMatrix;

void setupVertices()
{
    // 36 vertices, 12 triangles, makes 2x2x2 cube placed at origin.
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

void init(GLFWwindow* window)
{
    renderingProgram = Util::createShaderProgram("../src/shaders/vertex.glsl", "../src/shaders/fragment.glsl");

    cameraX = 0.0f;
    cameraY = 0.0f;
    cameraZ = 420.0f;

    setupVertices();
}

void display(GLFWwindow* window, double currentTime)
{
    glClear(GL_DEPTH_BUFFER_BIT);
    // glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(renderingProgram);

    // Get the uniform variables for the model-view and projection matrices.
    pLocation = glGetUniformLocation(renderingProgram, "pMatrix");
    vLocation = glGetUniformLocation(renderingProgram, "vMatrix");
    tfLocation = glGetUniformLocation(renderingProgram, "timeFactor");

    // Build perspective matrix
    glfwGetFramebufferSize(window, &width, &height);
    aspectRatio = (float)width / (float)height;
    pMatrix = glm::perspective(1.0472f, aspectRatio, 0.1f, 1000.0f);    // 1.0472 radians = 60 degrees

    float timeFactor = 0.0f;
    for (int i = 0; i < 24; i++)
    {
        timeFactor = currentTime + i;
        
        // Use current time to compute different translations in x, y and z.
        tMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(std::sin(0.35f * timeFactor) * 8.0f,
                                                            std::cos(0.52f * timeFactor) * 8.0f,
                                                            std::sin(0.70f * timeFactor) * 8.0f));
        rMatrix = glm::rotate(glm::mat4(1.0f), 1.75f * (float)currentTime, glm::vec3(0.0f, 1.0f, 0.0f));    //
        rMatrix = glm::rotate(rMatrix, 1.75f * (float)currentTime, glm::vec3(1.0f, 0.0f, 0.0f));            // the 1.75 adjusts the rotation speed
        rMatrix = glm::rotate(rMatrix, 1.75f * (float)currentTime, glm::vec3(0.0f, 0.0f, 1.0f));            //

        mMatrix = tMatrix * rMatrix;

        // Build view, model and model-view matrix.
        vMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
        mvMatrix = vMatrix * mMatrix;

        // Computations that build (and transform) mMatrix have been moved to othe vertex shader.
        // There is no longer any need to build a model-view matrix in the C++ application.
        glUniformMatrix4fv(pLocation, 1, GL_FALSE, glm::value_ptr(pMatrix));
        glUniformMatrix4fv(vLocation, 1, GL_FALSE, glm::value_ptr(vMatrix));
        glUniform1f(tfLocation, (float)timeFactor);


        // Associate VBO with the corresponding vertex attribute in the vertex shader.
        glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        // Adjust OpenGL settings and draw model.
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 25000);
    }
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

    GLFWwindow* window = glfwCreateWindow(600, 600, "TCC", nullptr, nullptr);
    if (!window)
    {
        const char* description = nullptr;
        int code = glfwGetError(&description);

        std::cerr << "Failed to create a GLFW window" << std::endl;

        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

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
    std::cout << std::endl;

    // Register a callback function on the window that
    // gets called each time the window is resized.
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    glfwSwapInterval(1);

    init(window);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        display(window, glfwGetTime());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
