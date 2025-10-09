#pragma once

#include "../PipelineState.hpp"
#include "../Shader.hpp"
#include "../text/Text.hpp"

class TextRenderer {
public:
    TextRenderer(PipelineState &pipeline_state, Vector2i window_size);

    TextRenderer(const TextRenderer &) = delete;
    TextRenderer& operator=(const TextRenderer&) = delete;

    void windowSize(Vector2i size);
    void render(Text &text, Vector2f position, Color color = Color{255, 255, 255});
private:
    PipelineState &pipeline_state_;
    Shader shader_;
    GLuint a_pos_location_;
    GLuint a_tex_coord_location_;
    GLuint u_window_size_location_;
    GLuint u_texture_size_location_;
    GLuint u_pos_location_;
    GLuint u_color_location_;
    GLuint u_scale_location_;
};

