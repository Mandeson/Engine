#version 120

varying vec2 texCoord;

uniform sampler2D sampler;
uniform vec3 uColor;

void main() {
    float val = texture2D(sampler, texCoord).r;
    gl_FragColor = vec4(uColor * val, val);
}