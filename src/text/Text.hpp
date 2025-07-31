#pragma once

#include <climits>
#include <condition_variable>
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
    Text(ThreadPool &thread_pool, Font &font, float font_size, const std::string &str, Alignment alignment = Alignment::kLeft, int max_width = INT_MAX);

    Text(const Text &) = delete;
    Text& operator=(const Text&) = delete;

    void setString(const std::string &str, Alignment alignment = Alignment::kLeft, int max_width = INT_MAX);
private:
    ThreadPool &thread_pool_;
    FontAtlas &font_atlas_;
    bool background_work_active_ = false;
    float font_size_;
    std::wstring str_;
    std::vector<std::unique_ptr<TextureBufferBuilder>> render_units_;
    std::mutex mutex_;
    std::condition_variable background_work_condition_;

    friend class TextRenderer;
};
