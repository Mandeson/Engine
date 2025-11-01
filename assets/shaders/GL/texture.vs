#version 120

attribute vec2 aPos;
attribute vec2 aTexCoord;

uniform vec2 uWindowSize;
uniform vec2 uTextureSize;
uniform vec2 uPos;
uniform vec2 uScale;

varying vec2 texCoord;

void main() {
   	gl_Position = vec4((uPos.x + aPos.x * uScale.x) / uWindowSize.x * 2.0 - 1.0,
            -((uPos.y + aPos.y * uScale.x) / uWindowSize.y * 2.0 - 1.0), 0.0, 1.0);
   	texCoord = aTexCoord * uTextureSize;
}
