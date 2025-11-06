#include "TextManager.hpp"

TextObject::TextObject(ThreadPool &thread_pool, Font &font, float font_size)
        : text_(std::make_shared<Text>( thread_pool, font, font_size)) { }

Text &TextObject::getText() {
    return *text_; // It's safe, because the pointer is initialized in the constroctor
}

void TextObject::setPos(Vector2d pos) {
    pos_ = pos;
}

void TextObject::setColor(Color color) {
    color_ = color;
}

Vector2d TextObject::getPos() {
    return pos_;
}

Color TextObject::getColor() {
    return color_;
}

TextId TextManager::newObject(ThreadPool &thread_pool, Font &font, float font_size) {
    TextId id = findEmptyOrCreate();
    objects_[id].emplace(thread_pool, font, font_size);
    return id;
}

TextObject &TextManager::getObject(TextId text_id) {
    return objects_.at(text_id).value();
}
