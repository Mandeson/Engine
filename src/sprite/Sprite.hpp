#pragma once

#include "SpriteBase.hpp"
#include "../Texture.hpp"
#include "../util/TextureRect.hpp"
#include "../util/Vector.hpp"

class Sprite : public SpriteBase {
public:
    Sprite(Texture &texture, TextureRect texture_rect);
    bool ready();
    Texture &getTexture();
    void setSize(Vector2i size);
private:
    Texture &texture_;
};
