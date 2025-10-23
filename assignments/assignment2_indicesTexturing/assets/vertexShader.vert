#version 330
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aUV;

out vec4 Color;
out vec2 UV;

uniform float uTime;
void main()
{
	Color = aColor;
	UV = aUV;
	vec3 pos = aPos * 1.778;

	//pos.y += sin((2 * uTime) + pos.x) * 0.3;
	//pos.x += cos((2 * uTime )+ pos.y) * 0.3;
	gl_Position = vec4(pos, 1.0);
}