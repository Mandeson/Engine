#pragma once

#include "OpenGL.hpp"

class PipelineState {
public:
    PipelineState() = default;
    ~PipelineState() = default;
    PipelineState(const PipelineState &) = delete;
    PipelineState &operator=(const PipelineState &) = delete;

    void useShader(GLuint shader);
    void bindTexture(GLuint texture);
private:
    GLuint bound_shader_ = 0;
    GLuint bound_texture_ = -1;
};
