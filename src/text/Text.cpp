#include "Text.hpp"
#include "../util/UTF-8.hpp"
#include "FontAtlas.hpp"
#include <memory>
#include <mutex>
#include <string>

Text::Text(ThreadPool &thread_pool, Font &font, float font_size, const std::string &str, Alignment alignment, int max_width)
        : thread_pool_(thread_pool), font_atlas_(font.getAtlas(font_size)), font_size_(font_size) {
    setString(str, alignment, max_width);
}

Text::~Text() {
    std::unique_lock lock(mutex_);
    background_work_condition_.wait(lock, [this]{
        return !background_work_active_;
    });
}

void Text::setString(const std::string &str, Alignment alignment, int max_width) {
    std::unique_lock lock(mutex_);
    background_work_condition_.wait(lock, [this]{
        return !background_work_active_;
    });
    background_work_active_ = true;
    UTF8::decode(str_, str);
    for (auto &buffer : render_units_)
        buffer->clear();
    thread_pool_.execute([this, alignment, max_width]{
        int line_height = static_cast<int>(font_size_ * 1.15f);
        int pen_x = 0;
        int pen_y = static_cast<int>(font_size_);
        size_t index = 0;
        int max_final_width = 0;
        do {
            int width = 0;
            int final_width = 0;
            size_t length = 0;
            do {
                int current_width = width;
                int current_final_width = width;
                size_t currrent_index = index;
                size_t current_length = length;
                while (1) {
                    wchar_t ch = str_[currrent_index];
                    currrent_index++;
                    current_length++;
                    if (ch == 0 || ch == '\n')
                        break;
                    try {
                        auto &glyph = font_atlas_.getGlyph(ch);
                        current_final_width = current_width + glyph.size.x + glyph.offset.x;
                        current_width += glyph.advance.x / 64;
                    } catch (FontAtlas::GlyphNotFoundError &) {
                        continue;
                    } catch (FontAtlas::GlyphRenderError &) {
                        continue;
                    }
                    if (str_[currrent_index - 1] == ' ')
                        break;
                };
                if (current_final_width > max_width && width != 0)
                    break;
                width = current_width;
                final_width = current_final_width;
                length = current_length;
                index = currrent_index;
            } while (str_[index - 1] != '\n' && str_[index - 1] != 0);
            length--;

            if (final_width > max_final_width)
                max_final_width = final_width;

            /*char s2[100];
            sprintf(s2, "fw: %d", finalWidth);
            logger_log(LOG_DEBUG, s2);*/

            if (alignment == Alignment::kCenter)
                pen_x -= final_width / 2;

            for (size_t i = 0; i < length; i++) {
                try {
                    auto &glyph = font_atlas_.getGlyph(str_[(index - length - 1) + i]);
                    if (static_cast<int>(render_units_.size()) <= glyph.texture_index)
                        render_units_.resize(glyph.texture_index + 1);
                    if(render_units_[glyph.texture_index] == nullptr)
                        render_units_[glyph.texture_index] = std::make_unique<TextureBufferBuilder>();
                    auto &buffer_builder = render_units_[glyph.texture_index];
                    buffer_builder->addRectangle(Vector2{pen_x + glyph.offset.x, pen_y - glyph.offset.y},
                            glyph.size, TextureRect{glyph.atlas_pos, glyph.size});
                    pen_x += glyph.advance.x / 64;
                    pen_y += glyph.advance.y / 64;
                } catch (FontAtlas::GlyphNotFoundError &) {
                    continue;
                } catch (FontAtlas::GlyphRenderError &) {
                    continue;
                }
            }

            pen_x = 0;
            pen_y += line_height;
        } while (index < str_.size());
        str_ = std::wstring();
        //text->_dimensions.width = maxFinalWidth;
        //text->_dimensions.height = penY - (int)(lineHeight * 0.8) - p->penYInit;
        for (auto &buffer : render_units_)
            buffer->end();
        {
            std::scoped_lock lock{mutex_};
            background_work_active_ = false;
        }
        background_work_condition_.notify_all();
    });
}
