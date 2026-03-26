#pragma once

#ifdef __ANDROID__
#include <glad/gles2.h> // OpenGL ES 2.0 required
#else
#include <glad/gl.h> // OpenGL 2.1 with GL_ARB_framebuffer_object required
#endif

namespace OpenGL {

bool isGLES();
bool vertexArraysSupported();
GLint getMonochromeTextureFormat();

} // namespace OpenGL
