#version 120

varying vec2 texCoord;

uniform float uPixelScale;
uniform vec2 uTextureSize; 
uniform sampler2D sampler;

void main() {
    // This shader samples pixels from the texture as if it was upscaled by uPixelScale
    // and then performs bilinear interpolation. Useful when rendering pixel art elements at
    // non-integer coordinates

    // We get integer coordinates of the four texels sorrounding the processed texture coordinate
    // for performance reasons, we use code below instead of:
    // vec2 texel1Ui = floor(floor(texCoord * uPixelScale + vec2(-0.5, -0.5)) / uPixelScale);
    // vec2 texel2Ui = floor(floor(texCoord * uPixelScale + vec2( 0.5, -0.5)) / uPixelScale);
    // vec2 texel1Di = floor(floor(texCoord * uPixelScale + vec2(-0.5,  0.5)) / uPixelScale);
    // vec2 texel2Di = floor(floor(texCoord * uPixelScale + vec2( 0.5,  0.5)) / uPixelScale);
    vec2 coord = (texCoord * uPixelScale) - vec2(0.5);
    vec2 coordi = floor(coord);
    vec2 coordFractionalPart = coord - coordi;
    vec2 texel1Ui = floor(coordi / uPixelScale);
    vec2 texel2Ui = floor(vec2(coordi.x + 1.0, coordi.y      ) / uPixelScale);
    vec2 texel1Di = floor(vec2(coordi.x      , coordi.y + 1.0) / uPixelScale);
    vec2 texel2Di = floor(vec2(coordi.x + 1.0, coordi.y + 1.0) / uPixelScale);

    // floor and adding 0.5 to round the coordinates to the texture pixel center
    vec4 texel1UColor = texture2D(sampler, (texel1Ui + vec2(0.5)) * uTextureSize);
    vec4 texel2UColor = texture2D(sampler, (texel2Ui + vec2(0.5)) * uTextureSize);
    vec4 texel1DColor = texture2D(sampler, (texel1Di + vec2(0.5)) * uTextureSize);
    vec4 texel2DColor = texture2D(sampler, (texel2Di + vec2(0.5)) * uTextureSize);

    // bilinear interpolation
    vec4 mixU = mix(texel1UColor, texel2UColor, coordFractionalPart.x);
    vec4 mixD = mix(texel1DColor, texel2DColor, coordFractionalPart.x);
    gl_FragColor = mix(mixU, mixD, coordFractionalPart.y);
}
