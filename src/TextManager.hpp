#pragma once

#include "ObjectManagerBase.hpp"
#include "text/Text.hpp"
#include "util/Color.hpp"
#include "util/Vector.hpp"
#include <memory>

using TextId = int;

class TextObject {
public:
    TextObject(ThreadPool &thread_pool, Font &font, float font_size);

    ~TextObject() = default;
    TextObject(const TextObject &) = delete;
    TextObject(TextObject &&) = default;
    TextObject &operator=(const TextObject &) = delete;
    TextObject &operator=(TextObject &&) = default;
    
    Text &getText();
    void setPos(Vector2d pos);
    void setColor(Color color);
    Vector2d getPos();
    Color getColor();
private:
    // Must be a pointer, because the Text object uses multithreading. std::vector, which
    // is used in ObjectManagerBase can move data when reallocating.
    std::unique_ptr<Text> text_;
    Vector2d pos_ = {0.0, 0.0};
    Color color_ = {255, 255, 255};
};

class TextManager : public ObjectManagerBase<TextObject, TextId> {
public:
    TextManager() = default;

    ~TextManager() = default;
    TextManager(const TextManager &) = delete;
    TextManager &operator=(const TextManager &) = delete;

    TextObject &get(TextId text_id);
};
