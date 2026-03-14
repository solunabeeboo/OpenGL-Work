#version 330
out vec4 FragColor;

in vec2 UV;
in vec3 Normal;
in vec3 FragPos;
in vec3 WorldPos;

uniform float uTime;
uniform vec2 uResolution;
uniform float uSize;

uniform sampler2D uTexture;
uniform sampler2D uFoamTex;

uniform float waterAlpha;
uniform float darkness;
uniform float artificialDepth;
uniform float foamSpeed;
uniform float waveSpeed;
uniform float frothedScale;
uniform float frothBrightness;

uniform vec3 uLightColor;
uniform float uAmbient;
uniform float uDiffuse;
uniform vec3 uLightPos;
uniform vec3 uViewPos;
uniform float uSpecularStrength;
uniform float uShininess;
uniform int uShading;

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



//Blinn-Phong Coded by Rafaela Solis, Modified for different shading modes
//and debugging by Scott Accetta
void main()
{
    //Water Shader
        // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = UV;
    //normalize to relative dimensions
    uv.y *= (uResolution.y / uResolution.x);
    uv *= uSize;

    //recalculation normals, gotten from someones presentation
    vec3 dPdx = dFdx(WorldPos);
    vec3 dPdy = dFdy(WorldPos);
    vec3 geometricNormal = normalize(cross(dPdx, dPdy));
    
    // Blend with original smooth normal for softer lighting
    vec3 smoothNormal = normalize(Normal); // This was your original interpolated normal
    vec3 recalculatedNormal = normalize(mix(geometricNormal, smoothNormal, 0.3)); 


    vec4 middleTex = vec4(texture(uTexture, uv).xyz, waterAlpha);

    vec4 darkenedTex = vec4((middleTex * darkness).xyz, 1.0);

    vec4 mixedDepth = mix(middleTex, darkenedTex, artificialDepth);


    float tbySpeed = waveSpeed - (sin(uTime * foamSpeed) * (waveSpeed / 2.0));
    vec2 tiledWaves;
    TilingAndOffset(uv, vec2(5, 2.74), vec2(tbySpeed), tiledWaves);
    float noisedWaves;
    GradientNoise(tiledWaves, frothedScale, noisedWaves);
    vec2 smoothedWaves = (1.0 - smoothstep(vec2(0), vec2(0.55, 1.0), vec2(noisedWaves))) / vec2(8.0, 2.0);

    vec4 foamTexture = texture(uFoamTex, uv) * frothBrightness;
    vec4 outColor = mix(mixedDepth, foamTexture, (smoothedWaves.x + smoothedWaves.y) / 2.0);
    vec4 finalCol = vec4(outColor.xyz, mixedDepth.w);

    //Using RadioButtons to switch between shading types LIGHTING
    if (uShading == 0)
    {
        // RG is UV coordinates, B is 0
        FragColor = vec4(UV.x, UV.y, 0.0, 1.0);
        return;
    }

    if (uShading == 1)
    {
        // RGB is absolute value of normal
        vec3 normalColor = abs(normalize(recalculatedNormal));
        FragColor = vec4(normalColor, 1.0);
        return;
    }

     if (uShading == 2)
     {
        //TEX END

        vec3 ambient = uAmbient * uLightColor;

        vec3 norm = normalize(recalculatedNormal);
        vec3 lightDir = normalize(uLightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = uDiffuse * diff * uLightColor;

        vec3 viewDir = normalize(uViewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);

        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(norm, halfwayDir), 0.0), uShininess);
        // float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); //old blinn spec
        vec3 specular = uSpecularStrength * spec * uLightColor;

        //vec3 result = (ambient + diffuse + specular) * texture(uTexture, uv).xyz;
        vec3 result = (ambient + diffuse + specular) * finalCol.xyz;
        FragColor = vec4(result, waterAlpha);
        return;
     }

     //Adding the ever so fun magenta color for debugging shading system
     //Bug fixed the magenta color should not appear anymore (if does shading system is wrong)
     FragColor = vec4(1.0, 0.0, 1.0, 1.0);

}