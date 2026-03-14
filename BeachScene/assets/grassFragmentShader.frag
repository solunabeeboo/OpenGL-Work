
// Primary Author: Jordan Coolbeth

#version 330 core

out vec4 FragColor;

in vec2 UV;
in float syncTime;
in vec3 normal;
in vec3 fragPos;
in float aHeight;
in vec3 pos;

uniform sampler2D uTexture;

uniform vec3 lightPos;
uniform vec3 ambientLightColor;
uniform vec3 diffuseLightColor;
uniform vec3 specularLightColor;
uniform vec3 viewPos;
uniform float ambientStrength;
uniform float diffuseStrength;
uniform float specularStrength;
uniform float shininess;

uniform int uShading;

uniform vec3 baseColor;
uniform float colorStrength;
uniform float brightnessStrength;

void main()
{	
    // Something I can actually edit
    vec2 UVa = UV;

    //Using RadioButtons to switch between shading types
    if (uShading == 0)
    {
        // RG is UV coordinates, B is 0
        FragColor = vec4(UV.x, UV.y, 0.0, 1.0);
        return;
    }

    if (uShading == 1)
    {
        // RGB is absolute value of normal
        vec3 normalColor = abs(normalize(normal));
        FragColor = vec4(normalColor, 1.0);
        return;
    }

    // Ambient Light
    vec4 ambient = vec4(ambientStrength * ambientLightColor, 1.0);

    // Diffuse light
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0) * diffuseStrength;
    vec4 diffuse = vec4((diff * diffuseLightColor), 1.0);

    // Specular light
    vec3 viewDir    = normalize(viewPos - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess) * specularStrength;
    vec4 specular = vec4(specularLightColor * spec, 1.0);

    // Alter color slightly based on position
    float colorVariation = cos(fragPos.x / 2) * sin(fragPos.x / 2) * cos(fragPos.z / 2) * sin(fragPos.z / 2);
    colorVariation *= colorStrength;

    // Get darker at bottom of blade
    float divisor = aHeight / (aHeight / 2);   
    float scaleCompensator = 8.0f;
    vec4 gradient = vec4(((pos.y / scaleCompensator) + (colorVariation * cos(fragPos.x))) * divisor, ((pos.y / scaleCompensator) + (colorVariation * cos(fragPos.y / scaleCompensator))) * divisor, ((pos.y / scaleCompensator) - (colorVariation * cos(fragPos.z))) * divisor, 0);

    // Alter brightness based on position
    float brightnessVariation = cos(fragPos.x * 2 * cos(fragPos.z * 2)) * sin(fragPos.x * 2 * sin(fragPos.z * 2));
    brightnessVariation *= brightnessStrength * 1.5;
    gradient.x += brightnessVariation;
    gradient.y += brightnessVariation;
    gradient.z += brightnessVariation;

    FragColor = (ambient + diffuse + specular) * texture(uTexture, UVa) * gradient * vec4(baseColor, 1.0);
}