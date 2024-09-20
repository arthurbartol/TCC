#version 420

in vec4 varyingColor;

out vec4 color;

uniform mat4 mvMatrix;
uniform mat4 pMatrix;

void main()
{
    color = varyingColor;
}
