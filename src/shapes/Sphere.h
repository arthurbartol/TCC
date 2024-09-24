#pragma once

#include <glm/glm.hpp>

#include <cmath>
#include <vector>

class Sphere
{
public:
    int numVertices;
    int numIndices;

    std::vector<int> indices;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;

    Sphere(int precision);

    float toRadians(float degrees);

private:
    void init(int precision);
};
