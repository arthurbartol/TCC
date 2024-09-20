#version 420

layout (location = 0) in vec3 position;

out vec4 varyingColor;

uniform mat4 mvMatrix;
uniform mat4 pMatrix;

void main()
{
    gl_Position = pMatrix * mvMatrix * vec4(position, 1.0);
    varyingColor = vec4(position, 1.0) * 0.5 + vec4(0.5, 0.5, 0.5, 0.5);
}
