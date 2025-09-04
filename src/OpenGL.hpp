#pragma once

#ifdef __ANDROID__
#include <GLES2/gl2.h> // OpenGL ES 2.0 required
#else
#include <glad/glad.h> // OpenGL 2.1 with GL_EXT_framebuffer_object required
#endif

namespace OpenGL {

bool isGLES();

extern void (*glBindFramebufferPtr)(GLenum, GLuint);
extern void (*glDeleteFramebuffersPtr)(GLsizei, const GLuint *);
extern void (*glGenFramebuffersPtr)(GLsizei, GLuint *);
extern GLenum (*glCheckFramebufferStatusPtr)(GLenum);
extern void (*glFramebufferTexture2DPtr)(GLenum, GLenum, GLenum, GLuint, GLint);

}
