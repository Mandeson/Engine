#version 100

precision highp float;

attribute vec2 aPos;
attribute vec2 aTexCoord;

uniform vec2 uWindowSize;
uniform float uTextureSize;
uniform vec2 uPos;

varying vec2 texCoord;

void main() {
   	gl_Position = vec4((aPos.x + uPos.x) / uWindowSize.x * 2.0 - 1.0,
            -((aPos.y + uPos.y) / uWindowSize.y * 2.0 - 1.0), 0.0, 1.0);
   	texCoord = aTexCoord * uTextureSize;
}
