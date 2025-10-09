#pragma once

#include "OpenGL.hpp"

class PipelineState {
public:
    void useShader(GLuint shader);
    void bindTexture(GLuint texture);
private:
    GLuint bound_shader_ = 0;
    GLuint bound_texture_ = -1;
};
