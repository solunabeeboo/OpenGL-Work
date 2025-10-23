#version 330
out vec4 FragColor;
in vec4 Color;
in vec2 UV;

uniform float uTime;
uniform sampler2D uTexture;
uniform vec2 uResolution;
uniform int uRepeat;

void main()
{             

   vec2 uv = UV;
   //normalize to relative dimensions
    uv.y *= (uResolution.y / uResolution.x);

   //scale down and position
   uv *= uRepeat;
   uv -= vec2(2.5, 1.5);

   //flip
   uv *= -1.0;

   //animate
   uv.y += sin(uTime) * 0.25;

    //FragColor = texColor;
   FragColor = texture(uTexture, uv);
}