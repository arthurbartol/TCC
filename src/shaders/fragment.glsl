#version 420

uniform mat4 pMatrix;
uniform mat4 mvMatrix;

in vec4 varyingColor;

out vec4 color;

void main()
{
    color = varyingColor;
}
