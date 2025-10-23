#version 330
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;
	
out vec4 Color;

uniform float uTime;
void main()
{
	Color = aColor;
	vec3 pos = aPos;
	pos.x /= 1.778;
	pos.y += sin((2 * uTime) + pos.x) * 0.3;
	pos.x += cos((2 * uTime )+ pos.y) * 0.3;
	gl_Position = vec4(pos, 1.0);
}