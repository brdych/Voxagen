#version 400

layout(location = 0) in vec3 position;
layout(location = 1) in float normal;
layout(location = 2) in vec3 color;

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
    fragmentCol = color;//vec3(0.1,0.4,0.1);
    fragNormal=(normal<0.1)?vec3(0,1,0):(normal<1.1)?vec3(0,-1,0):(normal<2.1)?vec3(1,0,0):(normal<3.1)?vec3(-1,0,0):(normal<4.1)?vec3(0,0,1):vec3(0,0,-1);
    fogInfo = FOG_INFO;
    clearCol = CLEAR_COL;
    lightDir = -normalize(GLOBAL_LIGHT_DIR);
    lightCol = GLOBAL_LIGHT_COL;
}
