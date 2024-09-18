#pragma once

#include <GL/glew.h>

#include <string>

class Util
{
public:
    static GLuint createShaderProgram(const char* vertexSource, const char* fragmentSource);

private:
    static std::string readShaderSource(const std::string& filePath);
    // static std::string readShaderSource(const char* filePath);

    static void printShaderLog(GLuint shader);
    static void printProgramLog(int program);

    static bool checkOpenGLError();
};
