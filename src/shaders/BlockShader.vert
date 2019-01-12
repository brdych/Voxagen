#version 400

layout(location = 0) in vec3 position;
layout(location = 1) in float normal;

out vec3 fragNormal;
out vec3 fragmentColor;

uniform mat4 MVP;

void main() 
{
    gl_Position = MVP * vec4(position,1);
    fragmentColor = vec3(0.1,0.5,0.1);

    if(normal < 0.2)        {fragNormal = vec3(0,1,0);}
    else if(normal < 1.2)   {fragNormal = vec3(0,-1,0);}
    else if(normal < 2.2)   {fragNormal = vec3(1,0,0);}
    else if(normal < 3.2)   {fragNormal = vec3(-1,0,0);}
    else if(normal < 4.2)   {fragNormal = vec3(0,0,1);}
    else if(normal < 5.2)   {fragNormal = vec3(0,0,-1);}
}
