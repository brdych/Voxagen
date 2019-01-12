#version 400
out vec3 colour;

in vec3 fragNormal;
in vec3 fragmentColor;

uniform vec3 GLOBAL_LIGHT_DIR;
uniform vec3 GLOBAL_LIGHT_COL;

void main() 
{
    vec3 nLightDir = -normalize(GLOBAL_LIGHT_DIR);

    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * GLOBAL_LIGHT_COL;

    float diff = max(dot(fragNormal, nLightDir), 0.0);
    vec3 diffuse = diff * GLOBAL_LIGHT_COL;

    vec3 result = (ambient+diffuse) * fragmentColor;
    colour = result;
}
