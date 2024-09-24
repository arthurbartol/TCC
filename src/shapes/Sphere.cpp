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

float Sphere::toRadians(float degrees)
{
    return (degrees * 2.0f * 3.14159f) / 360.0f;
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

    // Calculate triangle vertices
    for (int i = 0; i <= precision; i++)
    {
        for (int j = 0; j <= precision; j++)
        {
            float y =  (float)std::cos(toRadians(180.0f - i * 180.0f / precision));
            float x = -(float)std::cos(toRadians(j * 360.0f / precision)) * (float)std::abs(std::cos(std::asin(y)));
            float z =  (float)std::sin(toRadians(j * 360.0f / precision)) * (float)std::abs(std::cos(std::asin(y)));

            vertices [i * (precision + 1) + j] = glm::vec3(x, y, z);
            normals  [i * (precision + 1) + j] = glm::vec3(x, y, z);
            texCoords[i * (precision + 1) + j] = glm::vec2((float)j / precision, (float)i / precision);
        }
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
