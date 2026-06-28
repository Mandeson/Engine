#pragma once

#include "SpriteBase.hpp"
#include "../Texture.hpp"
#include "../util/TextureRect.hpp"
#include "../util/Vector.hpp"

class Sprite : public SpriteBase {
public:
    Sprite(Texture &texture, TextureRect texture_rect);

    ~Sprite() = default;
    Sprite(const Sprite &) = delete;
    Sprite(Sprite &&) noexcept = default;
    Sprite &operator=(const Sprite &) = delete;
    Sprite &operator=(Sprite &&) = delete;

    bool ready();
    Texture &getTexture();
    TextureRect getTextureRect();
    void setSize(Vector2i size);
private:
    Texture &texture_;
    TextureRect texture_rect_;
};
