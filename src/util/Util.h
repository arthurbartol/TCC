#pragma once

#include <GL/glew.h>

#include <string>

class Utils
{
public:
    static GLuint createShaderProgram(const char* vertexSource, const char* fragmentSource);

private:
    static std::string readShaderSource(const char* filePath);

    static void printShaderLog(GLuint shader);
    static void printProgramLog(int program);

    static bool checkOpenGLError();
};
