#version 330
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUV;

out vec4 Color;
out vec2 UV;

uniform float uTime;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	UV = aUV;
	vec3 pos = aPos;

	// note that we read the multiplication from right to left
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}