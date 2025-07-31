#pragma once

#include "../Shader.hpp"
#include "../text/Text.hpp"

class TextRenderer {
public:
    TextRenderer();

    TextRenderer(const TextRenderer &) = delete;
    TextRenderer& operator=(const TextRenderer&) = delete;

    void render(Text &text);
private:
    Shader shader_;
    GLuint a_pos_location_;
    GLuint a_tex_coord_location_;
    GLuint u_window_size_location_;
    GLuint u_texture_size_location_;
    GLuint u_pos_location_;
    GLuint u_color_location_;
};

