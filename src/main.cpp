#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "util/Util.h"
#include "shapes/Sphere.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

#define NUM_VAOs 1
#define NUM_VBOs 3

GLuint vaos[NUM_VAOs];
GLuint vbos[NUM_VBOs];
GLuint renderingProgram;

float cameraX, cameraY, cameraZ;
// float cubeLocationX, cubeLocationY, cubeLocationZ;
// float pyramidLocationX, pyramidLocationY, pyramidLocationZ;

GLuint mvLocation, pLocation, vLocation;
int width, height;
float aspectRatio;
glm::mat4 pMatrix, vMatrix, mMatrix, mvMatrix, tMatrix, rMatrix;

std::stack<glm::mat4> mvStack;

Sphere mySphere(48);
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
    std::clog << std::endl;

    // Register a callback function on the window that
    // gets called each time the window is resized.
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // Enable vsync
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

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    aspectRatio = (float)width / (float)height;
    glViewport(0, 0, width, height);
    pMatrix = glm::perspective(1.0472f, aspectRatio, 0.1f, 1000.0f);    // 1.0472 radians = 60 degrees
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void setupVertices()
{
    std::vector<int> indices = mySphere.indices;
    std::vector<glm::vec3> vertices = mySphere.vertices;
    std::vector<glm::vec3> normals = mySphere.normals;
    std::vector<glm::vec2> texCoords = mySphere.texCoords;

    std::vector<float> pvalues; // vertex positions
    std::vector<float> tvalues; // normal vectors
    std::vector<float> nvalues; // texture positions

    int numIndices = mySphere.numIndices;
    for (int i = 0; i < numIndices; i++)
    {
        pvalues.push_back(vertices[indices[i]].x);
        pvalues.push_back(vertices[indices[i]].y);
        pvalues.push_back(vertices[indices[i]].z);

        nvalues.push_back(normals[indices[i]].x);
        nvalues.push_back(normals[indices[i]].y);
        nvalues.push_back(normals[indices[i]].z);

        tvalues.push_back(texCoords[indices[i]].s);
        tvalues.push_back(texCoords[indices[i]].t);
    }

    glGenVertexArrays(NUM_VAOs, vaos);
    glBindVertexArray(vaos[0]);

    glGenBuffers(NUM_VBOs, vbos);

    // Put the vertices into buffer 0
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

    // Put the normals into buffer 1
    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);

    // Put the texture coordinates into buffer 2
    glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
    glBufferData(GL_ARRAY_BUFFER, tvalues.size() * 4, &tvalues[0], GL_STATIC_DRAW);
}

void init(GLFWwindow* window)
{
    renderingProgram = Util::createShaderProgram("../src/shaders/vertex.glsl", "../src/shaders/fragment.glsl");

    cameraX = 0.0f;
    cameraY = 0.0f;
    cameraZ = 8.0f;

    // cubeLocationX =  0.0f;
    // cubeLocationY = -2.0f;  // shift down Y to reveal perspective
    // cubeLocationZ =  0.0f;

    // pyramidLocationX = 0.0f;
    // pyramidLocationY = 2.0f;
    // pyramidLocationZ = 0.0f;

    setupVertices();

    glfwGetFramebufferSize(window, &width, &height);
    aspectRatio = (float)width / (float)height;
    pMatrix = glm::perspective(1.0472f, aspectRatio, 0.1f, 1000.0f);    // 1.0472 radians = 60 degrees
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

    // Use current time to compute different translations in x, y and z.
    // tMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(std::sin(0.35f * currentTime) * 8.0f,
    //                                                     std::cos(0.52f * currentTime) * 8.0f,
    //                                                     std::sin(0.70f * currentTime) * 8.0f));
    // rMatrix = glm::rotate(glm::mat4(1.0f), 1.75f * (float)currentTime, glm::vec3(0.0f, 1.0f, 0.0f));    // the 1.75
    // rMatrix = glm::rotate(        rMatrix, 1.75f * (float)currentTime, glm::vec3(1.0f, 0.0f, 0.0f));    // adjusts the
    // rMatrix = glm::rotate(        rMatrix, 1.75f * (float)currentTime, glm::vec3(0.0f, 0.0f, 1.0f));    // rotation speed

    // mMatrix = tMatrix * rMatrix;

    // The view matrix is computed once and used for both objects.
    vMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
    mvStack.push(vMatrix);

    glEnable(GL_CULL_FACE);

    glUniformMatrix4fv(mvLocation, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
    glUniformMatrix4fv(pLocation, 1, GL_FALSE, glm::value_ptr(pMatrix));
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDrawArrays(GL_TRIANGLES, 0, mySphere.numIndices);
}
