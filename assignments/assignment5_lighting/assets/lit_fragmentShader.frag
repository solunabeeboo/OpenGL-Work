#version 330
out vec4 FragColor;

in vec2 UV;
in vec3 Normal;
in vec3 FragPos;

uniform float uTime;
uniform vec2 uResolution;
uniform float uSize;
uniform sampler2D uTexture;

uniform vec3 uLightColor;
uniform float uAmbient;
uniform float uDiffuse;
uniform vec3 uLightPos;
uniform vec3 uViewPos;
uniform float uSpecularStrength;
uniform float uShininess;

void main()
{
        // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = UV;
    //normalize to relative dimensions
    uv.y *= (uResolution.y / uResolution.x);
    uv *= uSize;

    //TEX END

   vec3 ambient = uAmbient * uLightColor;

   vec3 norm = normalize(Normal);
   vec3 lightDir = normalize(uLightPos - FragPos);
   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = uDiffuse * diff * uLightColor;

   vec3 viewDir = normalize(uViewPos - FragPos);
   vec3 reflectDir = reflect(-lightDir, norm);

   vec3 halfwayDir = normalize(lightDir + viewDir);
   float spec = pow(max(dot(norm, halfwayDir), 0.0), uShininess);
   // float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); //old blinn spec
   vec3 specular = uSpecularStrength * spec * uLightColor;

   vec3 result = (ambient + diffuse + specular) * texture(uTexture, uv).xyz;
   FragColor = vec4(result, 1.0);
}