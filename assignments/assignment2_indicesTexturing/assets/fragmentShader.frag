#version 330
out vec4 FragColor;
in vec4 Color;
in vec2 UV;

uniform float uTime;
uniform float uOffset;
uniform vec3 uBaseColor;
uniform float uSize;

void main()
{
//originally coded in ShaderToy and i moved here
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = UV;
    //zoom the entire texture in or out
    uv *= uSize;

    //sin the addition of time by the uv coordinate on the x, and then scale and add user definer offset
    //which will squish or relax the background lines
    float d = sin(uTime+uv.x) * 0.5 + uOffset;

    // Time varying pixel color
    //multiply these lines by the cos of time and the uv's with x doubled, and a random vec3 offset added to it
    //then multiply this by a use defined "base color" which affects brightness, hue, and shape
    vec3 col = d * cos(uTime+uv.xyx+vec3(0,2,4)) * normalize(uBaseColor);

    //define s as a mod of uv.xy added and scaled, all finding the remainder by 2
    float s = mod(uv.x+uv.y*0.1, 2.0);
    //clamp this between 0.7 and 1.0 to limit dark shades
    s = clamp(s, 0.7, 1.0);

    //multiply these two together
    col = col * s;
    //mod one more time by 0.25 to get those weird vertical lines
    col = mod(col, 0.25);

    //set the outtext with one more fract to give the post-decimal portion
    vec4 outTex = fract(vec4(col * 4.0, 1.0));

    FragColor = vec4(outTex);
}