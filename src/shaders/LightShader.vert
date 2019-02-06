#version 400

layout(location = 0) in vec3 vertexPosition_modelspace;
out vec3 fragmentColor;
uniform mat4 MVP;
uniform vec3 COL;
void main() 
{
    gl_Position = MVP * vec4(vertexPosition_modelspace,1);
    fragmentColor = COL;
}
