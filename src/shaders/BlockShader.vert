#version 400

layout(location = 0) in vec3 position;
layout(location = 1) in float normal;

out vec3 fragNormal;
out vec3 fragmentCol;
out vec3 fogInfo;
out vec3 clearCol;
out vec3 lightDir;
out vec3 lightCol;

uniform mat4 MVP;
uniform vec3 FOG_INFO;
uniform vec3 CLEAR_COL;
uniform vec3 GLOBAL_LIGHT_DIR;
uniform vec3 GLOBAL_LIGHT_COL;

void main() 
{
    gl_Position = MVP * vec4(position,1);
    fragmentCol = vec3(0.1,0.4,0.1);

    if(normal      < 0.2)   {fragNormal = vec3(0,1,0);}
    else if(normal < 1.2)   {fragNormal = vec3(0,-1,0);}
    else if(normal < 2.2)   {fragNormal = vec3(1,0,0);}
    else if(normal < 3.2)   {fragNormal = vec3(-1,0,0);}
    else if(normal < 4.2)   {fragNormal = vec3(0,0,1);}
    else if(normal < 5.2)   {fragNormal = vec3(0,0,-1);}

    fogInfo = FOG_INFO;
    clearCol = CLEAR_COL;
    lightDir = -normalize(GLOBAL_LIGHT_DIR);
    lightCol = GLOBAL_LIGHT_COL;
}
