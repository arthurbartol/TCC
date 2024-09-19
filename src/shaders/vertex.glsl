#version 420

layout (location = 0) in vec3 position;

uniform mat4 pMatrix;
uniform mat4 mvMatrix;

out vec4 varyingColor;

void main()
{
    gl_Position = pMatrix * mvMatrix * vec4(position, 1.0f);
    varyingColor = vec4(position, 1.0f) * 0.5f + vec4(0.5f, 0.5f, 0.5f, 0.5f);
}
