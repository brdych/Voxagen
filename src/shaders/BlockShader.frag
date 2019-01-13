#version 400
out vec3 color;

in vec3 fragNormal;
in vec3 fragmentCol;
in vec3 fogInfo;
in vec3 clearCol;
in vec3 lightDir;
in vec3 lightCol;

void main() 
{
    //Fog
    float dist = gl_FragCoord.z / gl_FragCoord.w;
    float fogFactor = clamp((1.0/exp((dist*fogInfo.z)*(dist*fogInfo.z))),0.0,1.0);

    //Ambient Light
    float ambientStrength = 0.4;
    vec3 ambient = ambientStrength * lightCol;

    //Diffuse Light
    float diff = max(dot(fragNormal, lightDir), 0.0);
    vec3 diffuse = diff * lightCol;

    //Final
    vec3 lightColor = (ambient+diffuse) * fragmentCol;
    color = mix(clearCol, lightColor, fogFactor);
}
