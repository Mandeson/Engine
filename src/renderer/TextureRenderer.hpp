#pragma once

#include "../Shader.hpp"

class TextureRenderer {
public:
    struct ShaderLocations {
        GLuint a_pos_location_;
        GLuint a_tex_coord_location_;
        GLuint u_window_size_location_;
        GLuint u_texture_size_location_;
        GLuint u_pos_location_;
        GLuint u_scale_location_;
    };

    TextureRenderer();
    Shader &getShader();
    ShaderLocations &getShaderLocations();
private:
    Shader shader_;
    ShaderLocations shader_locations_;
};
