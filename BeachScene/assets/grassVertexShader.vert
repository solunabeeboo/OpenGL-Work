
// Primary Author: Jordan Coolbeth

#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 aNormal;

out vec2 UV;
out float syncTime;
out vec3 normal;
out vec3 fragPos;
out float aHeight;
out vec3 pos;

uniform float uTime;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float height;
uniform float windSpeed; 
uniform float windStrength; 
uniform float windDirection;

void main()
{
	// Set data
	syncTime = uTime;
	syncTime *= windSpeed;
	UV = aUV;
	pos = aPos;
	aHeight = height;
	normal = aNormal;
	float aStrength = height * windStrength;

	float directionComponent = syncTime + sqrt((cos(pos.x) * cos(pos.x)) + (sin(pos.z) * sin(pos.z)));
	float movement = (cos(directionComponent) + (windStrength / 2)) / 3 * aStrength;

	// Make grass sway
	if (pos.y > 0)
	{
		// Move back and forth
		float xChange = movement * cos(windDirection) * (pos.y / height);
		float zChange = movement * sin(windDirection) * (pos.y / height);
		pos.x += xChange;
		pos.z += zChange;

		//after wind deformation, tilt the normal based on displacement
		normal = normalize(aNormal + vec3(xChange * 0.5, 0, zChange * 0.5));

		// Adjust length so grass actually looks like it's rotating
		float lengthFactor = (height * height) - (xChange * xChange) - (zChange * zChange);
		if (lengthFactor > 0.0)
			 pos.y *= sqrt(lengthFactor) / height;
		else
			 pos.y *= 0.1; //fallback to prevent NaN
	}

	//normal = aNormal;
	fragPos = vec3(model * vec4(pos, 1.0));

	// Set position
	gl_Position = projection * view * model * vec4(pos, 1.0);
}