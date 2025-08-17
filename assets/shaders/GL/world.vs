#version 120

attribute vec2 aPos;
attribute vec2 aTexCoord;

uniform vec2 uWindowSize;
uniform vec2 uTextureSize;
uniform vec2 uCameraPos;
uniform float uTileSize;

varying vec2 texCoord;

void main() {
   	gl_Position = vec4((aPos.x * uTileSize - uCameraPos.x) / uWindowSize.x * 2.0 - 1.0,
            -((aPos.y * uTileSize - uCameraPos.y) / uWindowSize.y * 2.0 - 1.0), 0.0, 1.0);
   	texCoord = aTexCoord * uTextureSize;
}
