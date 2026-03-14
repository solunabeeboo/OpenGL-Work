#version 330
out vec4 FragColor;
in vec2 UV;

uniform vec3 uLightColor;

void main()
{
    FragColor = vec4(uLightColor, 1.0);
}