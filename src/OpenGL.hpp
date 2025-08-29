#pragma once

#ifdef __ANDROID__
#include <GLES2/gl2.h>
#define USE_GLES2 // OpenGL ES 2.0 required
#else
#include <glad/glad.h>
#define USE_GL // OpenGL 2.1 required
#endif
