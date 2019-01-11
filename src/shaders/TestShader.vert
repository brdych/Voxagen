#version 400

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in float normal;
out vec3 fragmentColor;
uniform mat4 MVP;
void main() 
{
    gl_Position = MVP * vec4(vertexPosition_modelspace,1);

    if(normal < 0.2)
    {
        fragmentColor = vec3(0.5,0.5,0.5);
    }
    else if(normal < 1.2)
    {
        fragmentColor = vec3(0.3,0.3,0.3);
    }
    else if(normal < 2.2)
    {
        fragmentColor = vec3(0.4,0.4,0.4);
    }
    else if(normal < 3.2)
    {
        fragmentColor = vec3(0.4,0.4,0.4);
    }
    else if(normal < 4.2)
    {
        fragmentColor = vec3(0.4,0.4,0.4);
    }
    else if(normal < 5.2)
    {
        fragmentColor = vec3(0.4,0.4,0.4);
    }
}
