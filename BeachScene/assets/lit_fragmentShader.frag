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
uniform int uShading;

//Blinn-Phong Coded by Rafaela Solis, Modified for different shading modes
//and debugging by Scott Accetta
void main()
{
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
        vec3 normalColor = abs(normalize(Normal));
        FragColor = vec4(normalColor, 1.0);
        return;
    }

     if (uShading == 2)
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
        return;
     }

     //Adding the ever so fun magenta color for debugging shading system
     //Bug fixed the magenta color should not appear anymore (if does shading system is wrong)
     FragColor = vec4(1.0, 0.0, 1.0, 1.0);

}