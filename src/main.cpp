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
#include <stack>

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
float cubeLocationX, cubeLocationY, cubeLocationZ;
float pyramidLocationX, pyramidLocationY, pyramidLocationZ;

GLuint mvLocation, pLocation, vLocation;
int width, height;
float aspectRatio;
glm::mat4 pMatrix, vMatrix, mMatrix, mvMatrix, tMatrix, rMatrix;

std::stack<glm::mat4> mvStack;

void setupVertices();

void init(GLFWwindow* window);
void display(GLFWwindow* window, double currentTime);

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

    // Pyramid with 18 vertices, composing 6 triangles (four sides, and two on the bottom)
    float pyramidPositions[54] = {
        -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  1.0f,  0.0f,   // front face
         1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  1.0f,  0.0f,   // right face
         1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  1.0f,  0.0f,   // back face
        -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  1.0f,  0.0f,   // left face
        -1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,   // base - left front
         1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f    // base - right back
    };

    glGenVertexArrays(NUM_VAOs, vaos);
    glBindVertexArray(vaos[0]);

    glGenBuffers(NUM_VBOs, vbos);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidPositions), pyramidPositions, GL_STATIC_DRAW);
}

void init(GLFWwindow* window)
{
    renderingProgram = Util::createShaderProgram("../src/shaders/vertex.glsl", "../src/shaders/fragment.glsl");

    cameraX = 0.0f;
    cameraY = 0.0f;
    cameraZ = 8.0f;

    cubeLocationX =  0.0f;
    cubeLocationY = -2.0f;  // shift down Y to reveal perspective
    cubeLocationZ =  0.0f;

    pyramidLocationX = 0.0f;
    pyramidLocationY = 2.0f;
    pyramidLocationZ = 0.0f;

    setupVertices();
}

void display(GLFWwindow* window, double currentTime)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glUseProgram(renderingProgram);

    // Get the uniform variables for the model-view and projection matrices.
    mvLocation = glGetUniformLocation(renderingProgram, "mvMatrix");
    pLocation = glGetUniformLocation(renderingProgram, "pMatrix");
    vLocation = glGetUniformLocation(renderingProgram, "vMatrix");

    // Build perspective matrix
    glfwGetFramebufferSize(window, &width, &height);
    aspectRatio = (float)width / (float)height;
    pMatrix = glm::perspective(1.0472f, aspectRatio, 0.1f, 1000.0f);    // 1.0472 radians = 60 degrees

    // Use current time to compute different translations in x, y and z.
    tMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(std::sin(0.35f * currentTime) * 8.0f,
                                                        std::cos(0.52f * currentTime) * 8.0f,
                                                        std::sin(0.70f * currentTime) * 8.0f));
    rMatrix = glm::rotate(glm::mat4(1.0f), 1.75f * (float)currentTime, glm::vec3(0.0f, 1.0f, 0.0f));    // the 1.75
    rMatrix = glm::rotate(        rMatrix, 1.75f * (float)currentTime, glm::vec3(1.0f, 0.0f, 0.0f));    // adjusts the
    rMatrix = glm::rotate(        rMatrix, 1.75f * (float)currentTime, glm::vec3(0.0f, 0.0f, 1.0f));    // rotation speed

    mMatrix = tMatrix * rMatrix;

    // The view matrix is computed once and used for both objects.
    vMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
    mvStack.push(vMatrix);

    //---------------------- pyramid == sun ----------------------
    mvStack.push(mvStack.top());
    mvStack.top() *= glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));                  // sun position

    mvStack.push(mvStack.top());
    mvStack.top() *= glm::rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(1.0f, 0.0f, 0.0f)); // sun rotation

    glUniformMatrix4fv(mvLocation, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
    glUniformMatrix4fv(pLocation, 1, GL_FALSE, glm::value_ptr(pMatrix));
    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // Adjust OpenGL settings and draw model.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, 18);

    mvStack.pop();  // remove the sun's axial rotation from the stack

    //---------------------- cube == planet ----------------------
    mvStack.push(mvStack.top());
    mvStack.top() *= glm::translate(
        glm::mat4(1.0f),
        glm::vec3(
            std::sin((float)currentTime) * 4.0f,
            0.0f,
            std::cos((float)currentTime) * 4.0f
        )
    );

    mvStack.push(mvStack.top());
    mvStack.top() *= glm::rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0f, 1.0f, 0.0f)); // planet rotation

    glUniformMatrix4fv(mvLocation, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
    glUniformMatrix4fv(pLocation, 1, GL_FALSE, glm::value_ptr(pMatrix));
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    mvStack.pop();  // remove the planet's axial rotation from the stack

    //---------------------- smaller cube == moon ----------------------
    mvStack.push(mvStack.top());
    mvStack.top() *= glm::translate(
        glm::mat4(1.0f),
        glm::vec3(
            0.0f,
            std::sin((float)currentTime) * 2.0f,
            std::cos((float)currentTime) * 2.0f
        )
    );
    mvStack.top() *= glm::rotate(glm::mat4(1.0f), (float)currentTime, glm::vec3(0.0f, 0.0f, 1.0f)); // moon rotation
    mvStack.top() *= glm::scale(glm::mat4(1.0f), glm::vec3(0.25f, 0.25f, 0.25f));                   // make the moon smaller

    glUniformMatrix4fv(mvLocation, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
    glUniformMatrix4fv(pLocation, 1, GL_FALSE, glm::value_ptr(pMatrix));
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Remove moon scale/rotation/position, planet position,
    // sun position, and view matrices from stack.
    mvStack.pop();
    mvStack.pop();
    mvStack.pop();
    mvStack.pop();
}
