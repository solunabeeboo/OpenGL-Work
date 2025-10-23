#version 330
out vec4 FragColor;
in vec4 Color;

uniform float uTime;
void main()
{
	vec4 modifiedColor = Color;

	float sinTime = sin(uTime);
	if (sinTime < 0)
		sinTime *= -1;

	modifiedColor *= sinTime + 0.5;
	FragColor = modifiedColor;
}