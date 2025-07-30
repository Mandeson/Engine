#include "TextureBufferBuilder.hpp"
#include "BufferBuilder.hpp"
#include "OpenGL.hpp"

void TextureBufferBuilder::addRectangle(Vector2<int16_t> pos, Vector2<int16_t> size, TextureRect texture_rect) {
    auto pos_d = pos + size;
    auto tex_pos = texture_rect.pos;
    auto tex_pos_d = texture_rect.pos + texture_rect.size;
    std::array<TextureBufferBuilderVertex, 4> vertices = {
        TextureBufferBuilderVertex{{pos.x,   pos.y  }, {tex_pos.x,   tex_pos.y  }},
        TextureBufferBuilderVertex{{pos_d.x, pos.y  }, {tex_pos_d.x, tex_pos.y  }},
        TextureBufferBuilderVertex{{pos_d.x, pos_d.y}, {tex_pos_d.x, tex_pos_d.y}},
        TextureBufferBuilderVertex{{pos.x,   pos_d.y}, {tex_pos.x,   tex_pos_d.y}}
    };
    addQuad(vertices);
}

bool TextureBufferBuilder::bind(GLuint a_pos_location, GLuint a_tex_coord_location, GLuint usage) {
    if(BufferBuilder::bind(usage)) { // if buffer is not empty
        glVertexAttribPointer(a_pos_location, 2, GL_SHORT, GL_FALSE, sizeof(TextureBufferBuilderVertex),
                reinterpret_cast<void *>(offsetof(TextureBufferBuilderVertex, pos)));
        glEnableVertexAttribArray(a_pos_location);
        glVertexAttribPointer(a_tex_coord_location, 2, GL_UNSIGNED_SHORT, GL_FALSE, sizeof(TextureBufferBuilderVertex),
                reinterpret_cast<void *>(offsetof(TextureBufferBuilderVertex, tex_coord)));
        glEnableVertexAttribArray(a_tex_coord_location);
        return true;
    } else {
        return false;
    }
}
