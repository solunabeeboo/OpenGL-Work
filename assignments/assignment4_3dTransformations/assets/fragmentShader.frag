#version 330
out vec4 FragColor;
in vec4 Color;
in vec2 UV;

uniform float uTime;
uniform vec2 uResolution;
uniform float uSize;

uniform sampler2D uTexture;

void main()
{
//originally coded in ShaderToy and i moved here
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = UV;

    uv.y *= (uResolution.y / uResolution.x);
    uv *= uSize;

    FragColor = texture(uTexture, uv);
}