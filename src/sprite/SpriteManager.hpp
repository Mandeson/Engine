#pragma once

#include "../ObjectManagerBase.hpp"
#include "Sprite.hpp"

using SpriteId = int;

class SpriteManager : public ObjectManagerBase<Sprite, SpriteId> {
public:
    SpriteId newObject(Texture &texture, TextureRect texture_rect);
    bool ready();
    
    void setPos(SpriteId sprite_id, Vector2d pos);
    void setSize(SpriteId sprite_id, Vector2i size);
    Vector2d getPos(SpriteId sprite_id);
    void move(SpriteId sprite_id, Vector2d move);
};
