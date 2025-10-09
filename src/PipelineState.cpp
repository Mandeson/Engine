#include "PipelineState.hpp"

void PipelineState::useShader(GLuint shader) {
    if (bound_shader_ != shader) {  // Avoid calling glUseProgram if the shader is already bound
        glUseProgram(shader);
        bound_shader_ = shader;
    }
}

void PipelineState::bindTexture(GLuint texture) {
    if (bound_texture_ != texture) { // Avoid binding if the texture is already bound
        glBindTexture(GL_TEXTURE_2D, texture);
        bound_texture_ = texture;
    }
}
