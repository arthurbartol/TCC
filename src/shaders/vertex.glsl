#version 420

layout (location = 0) in vec3 position;

uniform mat4 pMatrix;
uniform mat4 vMatrix;
uniform float timeFactor;   // time factor for animation and placement of cubes

out vec4 varyingColor;

// mat4 buildRotateX(float rad);
// mat4 buildRotateY(float rad);
// mat4 buildRotateZ(float rad);
// mat4 buildTranslate(float x, float y, float z);

mat4 buildRotateX(float rad)
{
    return mat4(1.0,      0.0,       0.0, 0.0,
                0.0, cos(rad), -sin(rad), 0.0,
                0.0, sin(rad),  cos(rad), 0.0,
                0.0,      0.0,       0.0, 1.0);
}

mat4 buildRotateY(float rad)
{
    return mat4( cos(rad), 0.0, sin(rad), 0.0,
                      0.0, 1.0,      0.0, 0.0,
                -sin(rad), 0.0, cos(rad), 0.0,
                      0.0, 0.0,      0.0, 1.0);
}

mat4 buildRotateZ(float rad)
{
    return mat4(cos(rad), -sin(rad), 0.0, 0.0,
                sin(rad),  cos(rad), 0.0, 0.0,
                     0.0,       0.0, 1.0, 0.0,
                     0.0,       0.0, 0.0, 1.0);
}

mat4 buildTranslate(float x, float y, float z)
{
    return mat4(1.0, 0.0, 0.0, 0.0,
                0.0, 1.0, 0.0, 0.0,
                0.0, 0.0, 1.0, 0.0,
                  x,   y,   z, 1.0);
}

void main()
{
    float id = gl_InstanceID + timeFactor;          // value based on time factor, but different for each cube instance
    float a = sin(203.0 * id / 8000.0) * 403.0;     // these are the x, y and z components for the translation below
    float b = cos(301.0 * id / 4001.0) * 401.0;
    float c = sin(400.0 * id / 6003.0) * 405.0;

    // Build the rotation and translation matrices to be applied to this cube's model matrix.
    mat4 localRotationX = buildRotateX(1000.0 * id);
    mat4 localRotationY = buildRotateY(1000.0 * id);
    mat4 localRotationZ = buildRotateZ(1000.0 * id);
    mat4 localTranslate = buildTranslate(a, b, c);

    // Build the model matrix and then the model-view matrix.
    mat4 newModelMatrix = localTranslate * localRotationX * localRotationY * localRotationZ;
    mat4 mvMatrix = vMatrix * newModelMatrix;
    
    gl_Position = pMatrix * mvMatrix * vec4(position, 1.0);
    varyingColor = vec4(position, 1.0) * 0.5 + vec4(0.5, 0.5, 0.5, 0.5);
}
