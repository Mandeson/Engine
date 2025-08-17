#pragma once

#include "BufferBuilder.hpp"
#include "OpenGL.hpp"
#include "util/TextureRect.hpp"
#include "util/Vector.hpp"
#include <cstdint>

struct TextureBufferBuilderVertex {
    Vector2<int16_t> pos;
    Vector2<uint16_t> tex_coord;
};

class TextureBufferBuilder : public BufferBuilder<TextureBufferBuilderVertex> {
public:
    void addRectangle(Vector2<int16_t> pos, Vector2<int16_t> size, TextureRect texture_rect, bool horizontal_flip=false, bool vertical_flip=false);
    bool bind(GLuint a_pos_location, GLuint a_tex_coord_location, GLuint usage = GL_DYNAMIC_DRAW);
};
