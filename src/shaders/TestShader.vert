#version 400

layout(location = 0) in vec3 vertexPosition_modelspace;
out vec3 fragmentColor;
uniform mat4 MVP;
void main() 
{
    gl_Position = MVP * vec4(vertexPosition_modelspace,1);

    float r = vertexPosition_modelspace.x/16;
    float g = vertexPosition_modelspace.y/16;
    float b = vertexPosition_modelspace.z/16;

    fragmentColor = vec3(r,g,b);
}
