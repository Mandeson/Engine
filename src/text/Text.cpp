#include "Text.hpp"
#include "../util/UTF-8.hpp"
#include "FontAtlas.hpp"
#include <memory>
#include <mutex>
#include <string>

Text::Text(ThreadPool &thread_pool, Font &font, float font_size)
        : thread_pool_(thread_pool), font_atlas_(font.getAtlas(font_size)), font_size_(font_size) { }

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
    {
        std::unique_lock lock(mutex_);
        background_work_condition_.wait(lock, [this]{
            return !background_work_active_;
        });
        background_work_active_ = true;
        UTF8::decode(str_, str);
        for (auto &buffer : render_units_)
            buffer->clear();
    }

    if (!stream_draw_) {
        if (render_count_since_update_ == 1) {
            Log::info("Text: using stream draw");
            stream_draw_ = true;
            render_count_since_update_ = 0;
        }

        thread_pool_.execute([this, alignment, max_width]{
            generateText(alignment, max_width);
        });
    } else { // If stream draw enabled, generate the text on the current thread
        generateText(alignment, max_width);
    }

    render_count_since_update_ = 0;
}

void Text::setScale(float scale) {
    scale_ = scale;
}

void Text::generateText(Text::Alignment alignment, int max_width) {
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
                { // adding, constructing elements and resizing a vector that may be read by the main thread
                    std::scoped_lock lock{mutex_};
                    if (static_cast<int>(render_units_.size()) <= glyph.texture_index)
                        render_units_.resize(glyph.texture_index + 1);
                    if (render_units_[glyph.texture_index] == nullptr)
                        render_units_[glyph.texture_index] = std::make_unique<TextureBufferBuilder>();
                }
                auto &buffer_builder = render_units_[glyph.texture_index];
                // addRectangle is thread safe, the atomic flag inside BufferBuilder guards its state
                buffer_builder->addRectangle(Vector2{pen_x + glyph.offset.x - 1, pen_y - glyph.offset.y - 1},
                        glyph.size + Vector2{2, 2}, TextureRect{glyph.atlas_pos - Vector2{1, 1},
                        glyph.size + Vector2{2, 2}}); // margins of 1px at each side to prevent sharp edges when using float coordinates
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
    {
        std::scoped_lock lock{mutex_};
        for (auto &buffer : render_units_) // After mutex lock to avoid partially rendered text
            buffer->end();
        background_work_active_ = false;
    }
    background_work_condition_.notify_all();
}

void Text::increaseRenderCount() {
    if (!stream_draw_ && render_count_since_update_ < 0xFF) // Max uint8_t value
        render_count_since_update_++;
}
