#version 330
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;

//out vec4 Color;
out vec2 UV;
out vec3 Normal;
out vec3 FragPos;
out vec3 WorldPos;

uniform float uTime;
uniform float uSpeed;
uniform float uFrequency;
uniform float uAmplitude;
uniform float uFloor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void Rotate(vec2 uv, vec2 center, float rotation, out vec2 Out)
{

	rotation = rotation * (3.1415926f/180.0f);
	uv -= center;
	float s = sin(rotation);
	float c = cos(rotation);
	mat2 rMatrix = mat2(c, -s, s, c);
	rMatrix *= 0.5f; //X
	rMatrix += 0.5f; //X
	rMatrix = rMatrix * 2.0f - 1.0f; //X
	//multiplt the uv and the rMatrix https://docs.unity3d.com/Packages/com.unity.shadergraph@6.9/manual/Rotate-Node.html
	//https://shadergraph.stelabouras.com/
	uv = rMatrix * uv;
	uv += center;
	Out = uv;
}

void TilingAndOffset(vec2 uv, vec2 tiling, vec2 offset, out vec2 Out)
{
	Out = uv * tiling + offset;
}

vec2 gradientNoiseBase(vec2 p)
{
	p = mod(p, 289.0f);
	float x = mod((34.0f * p.x + 1.0f) * p.x, 289.0f) + p.y;
	x = mod((34.0f * x + 1.0f) * x, 289.0f);
	x = fract(x / 41.0f) * 2.0f - 1.0f;
	
	return normalize(vec2(x - floor(x + 0.5), abs(x) - 0.5f));
}
float gradientNoiseHandler(vec2 p)
{

	vec2 ip = floor(p);
	vec2 fp = fract(p);
	float d00 = dot(gradientNoiseBase(ip), fp);
	float d01 = dot(gradientNoiseBase(ip + vec2(0,1)), fp - vec2(0,1));
	float d10 = dot(gradientNoiseBase(ip + vec2(1,0)), fp - vec2(1,0));
	float d11 = dot(gradientNoiseBase(ip + vec2(1,1)), fp - vec2(1,1));
	fp = fp * fp * fp * (fp * (fp * 6.0f - 15.0f) + 10.0f);

	return mix(mix(d00, d01, fp.y), mix(d10, d11, fp.y), fp.x);

}
void GradientNoise(vec2 uv, float scale, out float Out)
{	
	Out = gradientNoiseHandler(uv * scale) + 0.5f;
}

void WaveGenerator(float uv, vec2 freqMod, vec2 amplMod, out float Out)
{
	vec2 tilingOffset1;
	TilingAndOffset(vec2(uv), uFrequency / freqMod, vec2(0, 0), tilingOffset1);
	float gradientNoise1;
	GradientNoise(tilingOffset1, 10.0f, gradientNoise1);

	vec2 prime = amplMod * uAmplitude;
	float outMultiply2 = gradientNoise1 * (prime.x + prime.y);
	Out = clamp(outMultiply2, uFloor, outMultiply2);

}
void WaveGenerator(float uv, out float Out)
{
	WaveGenerator(uv, vec2(1.0f), vec2(1.0f), Out);
}

//Coded by Rafaela Solis
void main()
{
	//top branch
	UV = aUV;
	vec3 pos = aPos;

	vec3 transferPos = vec3(pos.x, pos.z, 0);

	//twist waves!
	float rotationVariation = (pos.x + pos.z) * 10.0;
    vec2 rotatedUV;
    Rotate(transferPos.xy, vec2(0.5f, 0.5f), 45.0f + rotationVariation, rotatedUV);

	//bottom branch
	float outMultiply = uSpeed * uTime;

	//Branches ADDED - unity is adding a vec2 and a float output a float in add node. Possible error in assumption here.
	float outUVAdd = (rotatedUV + outMultiply).x + (rotatedUV + outMultiply).y;

	//Now the THREE waves applying over the plane
		//WAVE 1
	float wave1;
	WaveGenerator(outUVAdd, wave1);

		//WAVE 2
	float wave2;
	WaveGenerator(outUVAdd, vec2(2.0f), vec2(1.5f, 2.0f), wave2);

		//WAVE 3
	float wave3;
	WaveGenerator(outUVAdd, vec2(3.0f, 2.0f), vec2(2.0f), wave3);

	//Adding waves to one
	float wave123 = (wave1 + wave2) + wave3;

	//align to the up and down vector ONLY. waves only rise and fall!
	vec3 applicableWave = vec3(0, wave123, 0);

	//Apply the vertex effect to the waves!
	vec3 ultimatePos = pos + applicableWave;

	// note that we read the multiplication from right to left
	gl_Position = projection * view * model * vec4(ultimatePos, 1.0);
	FragPos = vec3(model * vec4(ultimatePos, 1.0));
	WorldPos = FragPos;
	Normal = mat3(transpose(inverse(model))) * aNormal;
}