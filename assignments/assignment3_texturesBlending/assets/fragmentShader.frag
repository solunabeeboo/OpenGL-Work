#version 330
out vec4 FragColor;
in vec4 Color;
in vec2 UV;

uniform float uTime;
uniform sampler2D uTexture;
uniform sampler2D uTexture2;
uniform vec2 uResolution;
uniform int uRepeat;

void main()
{
    //repeat texture
    vec2 uv = UV * uRepeat;
    float bgSpeed = 1.5;
    float topSquiggleOffset = 3.0;
    vec2 circlingAnim;

    //scale texture so its not stretch but turning it back to square
    //  relative to the resolution
    uv.y *= (uResolution.y / uResolution.x);

   //normalized sin and cos
    float normalSin = sin(uTime * bgSpeed);
    if (normalSin < 0)
        normalSin *= -1;
    float normalCos = cos(uTime * bgSpeed);
    if (normalCos < 0)
        normalCos *= -1;

   //move around the center of the effect
    circlingAnim = vec2(-cos(uTime), -sin(uTime)) * 1.0;


   //make shoving line effects on the x and y
    uv.x += clamp(uv.y * normalSin, 0, 1) + circlingAnim.y;
    uv.x -= clamp((uv.y - topSquiggleOffset) * normalSin, 0, 1) + circlingAnim.y;

    uv.y += clamp(uv.x * normalCos, 0, 1) + circlingAnim.x;
    uv.y -= clamp((uv.x - topSquiggleOffset) * normalCos, 0, 1) + circlingAnim.x;

    //FragColor = vec4(UV.xyxx);
    FragColor = mix(texture(uTexture, uv), texture(uTexture2, UV), 0.2);
}