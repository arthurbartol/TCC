#include "include/Utils.h"

#include <iostream>
#include <fstream>

GLuint Utils::createShaderProgram(const char* vertexSource, const char* fragmentSource)
{
    GLint vertexCompiled, fragmentCompiled;
    GLint linked;

    std::string vertexShaderStr = readShaderSource(vertexSource);
    std::string fragmentShaderStr = readShaderSource(fragmentSource);

    const char* vertexShaderSrc = vertexShaderStr.c_str();
    const char* fragmentShaderSrc = fragmentShaderStr.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    GLuint vfProgramID = glCreateProgram();

    glShaderSource(vertexShader, 1, &vertexShaderSrc, nullptr);
    glShaderSource(fragmentShader, 1, &fragmentShaderSrc, nullptr);

    // Check for errors while compiling shaders.
    glCompileShader(vertexShader);
    checkOpenGLError();
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexCompiled);
    if (vertexCompiled != 1)
    {
        std::cerr << "Vertex Shader compilation failed." << '\n';
        printShaderLog(vertexShader);
    }

    glCompileShader(fragmentShader);
    checkOpenGLError();
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentCompiled);
    if (!fragmentCompiled)
    {
        std::cerr << "Fragment Shader compilation failed." << '\n';
        printShaderLog(fragmentShader);
    }

    // Catch errors while linking shaders.
    glAttachShader(vfProgramID, vertexShader);
    glAttachShader(vfProgramID, fragmentShader);

    glLinkProgram(vfProgramID);
    checkOpenGLError();
    glGetProgramiv(vfProgramID, GL_LINK_STATUS, &linked);
    if (!linked)
    {
        std::cerr << "Linking Shader Program failed." << '\n';
        printProgramLog(vfProgramID);
    }

    return vfProgramID;
}

std::string Utils::readShaderSource(const char* filePath)
{
    std::string content, line = "";
    std::ifstream fileStream(filePath, std::ios::in);

    while (fileStream.eof())
    {
        std::getline(fileStream, line);
        content.append(line + '\n');
    }

    fileStream.close();
    return content;
}

void Utils::printShaderLog(GLuint shader)
{
    int length = 0;
    int charWritten = 0;

    char* log;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    if (length > 0)
    {
        log = (char*)malloc(length);
        glGetShaderInfoLog(shader, length, &charWritten, log);
        std::clog << "Shader Info Log: " << log << '\n';
        free(log);
    }
}

void Utils::printProgramLog(int program)
{
    int length = 0;
    int charWritten = 0;

    char* log;

    glGetShaderiv(program, GL_INFO_LOG_LENGTH, &length);
    if (length > 0)
    {
        log = (char*)malloc(length);
        glGetShaderInfoLog(program, length, &charWritten, log);
        std::clog << "Program Info Log: " << log << '\n';
        free(log);
    }
}

bool Utils::checkOpenGLError()
{
    bool foundError = false;
    int glError = glGetError();

    while (glError != GL_NO_ERROR)
    {
        std::cerr << "glError: " << glError << '\n';
        foundError = true;
        glError = glGetError();
    }

    return foundError;
}
