#pragma once

#include <climits>
#include <condition_variable>
#include <cstdint>
#include <memory>
#include <mutex>
#include <string>
#include <vector>
#include "../TextureBufferBuilder.hpp"
#include "Font.hpp"
#include "FontAtlas.hpp"
#include "../ThreadPool.hpp"

class TextRenderer;

class Text {
public:
    enum class Alignment {
        kLeft,
        kCenter
    };
    Text(ThreadPool &thread_pool, Font &font, float font_size);
    Text(ThreadPool &thread_pool, Font &font, float font_size, const std::string &str, Alignment alignment = Alignment::kLeft, int max_width = INT_MAX);

    Text(const Text &) = delete;
    Text& operator=(const Text&) = delete;

    ~Text();

    void setString(const std::string &str, Alignment alignment = Alignment::kLeft, int max_width = INT_MAX);
    void setScale(float scale);
private:
    void generateText(Text::Alignment alignment, int max_width);
    void increaseRenderCount();

    ThreadPool &thread_pool_;
    FontAtlas &font_atlas_;
    bool background_work_active_ = false;
    float font_size_;
    std::wstring str_;

    // must use unique_ptr, because TextureBufferBuilder cannot be moved (it contains an atomic field)
    std::vector<std::unique_ptr<TextureBufferBuilder>> render_units_;
    
    std::mutex mutex_;
    std::condition_variable background_work_condition_;

    // Set when the engine detects that the text is changed every frame. Disables multithreading.
    bool stream_draw_ = false;
    uint8_t render_count_since_update_ = 0;

    float scale_ = 1.0f;

    friend class TextRenderer;
};
