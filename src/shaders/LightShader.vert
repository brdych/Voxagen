#version 400

layout(location = 0) in vec3 vertices;
out vec4 fragmentColor;
uniform mat4 MVP;
uniform vec3 COL;
void main() 
{
    gl_Position = MVP * vec4(vertices,1);
    fragmentColor = vec4(COL.xyz, 1.0f);
}
