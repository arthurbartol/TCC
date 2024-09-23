#include "Sphere.h"

#include <iostream>

Sphere::Sphere()
{
    init(48);
}

Sphere::Sphere(int precision)
{
    init(precision);
}

void Sphere::init(int precision)
{
    numVertices = (precision + 1) * (precision + 1);
    numIndices = precision * precision * 6;

    for (int i = 0; i < numVertices; i++)
    {
        vertices.push_back(glm::vec3());
        normals.push_back(glm::vec3());
        texCoords.push_back(glm::vec2());
    }

    for (int i = 0; i < numIndices; i++)
    {
        indices.push_back(0);
    }

    // Calculate triangle indices
    for (int i = 0; i < precision; i++)
    {
        for (int j = 0; j < precision; j++)
        {
            indices[6 * (i * precision + j) + 0] =       i * (precision + 1) + j;
            indices[6 * (i * precision + j) + 1] =       i * (precision + 1) + j + 1;
            indices[6 * (i * precision + j) + 2] = (i + 1) * (precision + 1) + j;
            indices[6 * (i * precision + j) + 3] =       i * (precision + 1) + j + 1;
            indices[6 * (i * precision + j) + 4] = (i + 1) * (precision + 1) + j + 1;
            indices[6 * (i * precision + j) + 5] = (i + 1) * (precision + 1) + j;
        }
    }
}

float Sphere::toRadians(float degrees)
{
    return (degrees * 2.0f * 3.14159f) / 360.0f;
}
