#pragma once

#include "SpriteManagerBase.hpp"
#include "Sprite.hpp"
#include "../Texture.hpp"

using SpriteId = int;

class SpriteManager : public SpriteManagerBase<Sprite, SpriteId> {
public:
    SpriteId newSprite(Texture &texture, TextureRect texture_rect);
    
    void setPos(SpriteId sprite_id, Vector2d pos);
    void setSize(SpriteId sprite_id, Vector2i size);
    Vector2d getPos(SpriteId sprite_id);
    void move(SpriteId sprite_id, Vector2d move);
};
