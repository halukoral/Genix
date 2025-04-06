﻿#version 450 core

const float gamma     = 2.2;
const float exposure  = 1.0;
const float pureWhite = 1.0;

layout(location = 0) in  vec2 screenPosition;
layout(binding = 0) uniform sampler2D sceneColor;
layout(location=0) out vec4 outColor;

void main()
{
    vec3 color = texture(sceneColor, screenPosition).rgb * exposure;

    // Reinhard tonemapping operator.
    // see: "Photographic Tone Reproduction for Digital Images", eq. 4
    float luminance = dot(color, vec3(0.2126, 0.7152, 0.0722));
    float mappedLuminance = (luminance * (1.0 + luminance/(pureWhite*pureWhite))) / (1.0 + luminance);

    // Scale color by ratio of average luminances.
    vec3 mappedColor = (mappedLuminance / luminance) * color;

    // Gamma correction.
    outColor = vec4(pow(mappedColor, vec3(1.0/gamma)), 1.0);
}
