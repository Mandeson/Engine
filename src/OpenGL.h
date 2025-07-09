#pragma once

#define USE_GL // OpenGL 2.1 required
//#define USE_GLES2 // OpenGL ES 2.0 required

#ifdef __ANDROID__
#include <GLES2/gl2.h>
#else
#include <glad/glad.h>
#endif
