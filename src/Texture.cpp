#include "Texture.hpp"
#include <cstdint>
#include <cstring>
#include "OpenGL.hpp"
#include "upng/upng.h"
#include "util/Logger.hpp"

Texture::FileNotFoundError::FileNotFoundError(std::string &&filename)
        : message_(std::format("Texture file not found: {}", filename)) { }

const char* Texture::FileNotFoundError::what() const noexcept {
    return message_.c_str();
}

Texture::DecodingError::DecodingError(std::string &&filename)
        : message_(std::format("Texture decoding error: {}", filename)) { }

const char* Texture::DecodingError::what() const noexcept {
    return message_.c_str();
}

Texture::Texture(ThreadPool &thread_pool, const std::string &filename)
        : background_work_active_(true) {
    thread_pool.execute([this, filename] {
        Log::dbg("Loading texture {}", filename);
        try {
            loadImpl(filename);
        } catch (...) {
            exception_ = std::current_exception();
            error_ = true;
            {
                std::scoped_lock lock{mutex_};
                background_work_active_ = false;
            }
            background_work_condition_.notify_all();
            return;
        }
        Log::dbg("Loaded texture {}", filename);
        ready_ = true;
        {
            std::scoped_lock lock{mutex_};
            background_work_active_ = false;
        }
        background_work_condition_.notify_all();
    });
}

Texture::~Texture() {
    std::unique_lock lock(mutex_);
    background_work_condition_.wait(lock, [this]{
        return !background_work_active_;
    });
    if (uploaded_)
        glDeleteTextures(1, &texture_id_);
    Log::dbg("texture destructor");
}

bool Texture::ready() {
    if (error_)
        std::rethrow_exception(exception_);

    return ready_;
}

void Texture::bind() {
    if (!ready())
        return;
    if (!uploaded_) {
        glGenTextures(1, &texture_id_);
        glBindTexture(GL_TEXTURE_2D, texture_id_);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        GLuint format = alpha_ ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, size_.x,
                size_.y, 0, format, GL_UNSIGNED_BYTE,
                reinterpret_cast<void *>(&pixel_buffer_[0]));
        uploaded_ = true;
    } else {
        glBindTexture(GL_TEXTURE_2D, texture_id_);
    }
}

Vector2i Texture::getSize() {
    return size_;
}

void Texture::decode(std::string &&filename, upng_t *upng) {
    upng_decode(upng);
    if (upng_get_error(upng) != UPNG_EOK)
        throw DecodingError(std::move(filename));
    if (upng_get_format(upng) ==  UPNG_RGBA8)
        alpha_ = true;
    size_ = Vector2{upng_get_width(upng), upng_get_height(upng)};
    if (alpha_) {
        size_t buffer_size = size_.x * size_.y * 4 * sizeof(uint8_t);
        pixel_buffer_.resize(buffer_size);
        std::memcpy(&pixel_buffer_[0], reinterpret_cast<const void *>(upng_get_buffer(upng)), buffer_size);
        // Premultiplication
        for (size_t i = 0; i < buffer_size - 1; i += 4) {
            float a = pixel_buffer_[i + 3] * (1.0f / 255);
            pixel_buffer_[i] = static_cast<uint8_t>(pixel_buffer_[i] * a);
            pixel_buffer_[i + 1] = static_cast<uint8_t>(pixel_buffer_[i + 1] * a);
            pixel_buffer_[i + 2] = static_cast<uint8_t>(pixel_buffer_[i + 2] * a);
        }
    } else {
        size_t buffer_size = size_.x * size_.y * 3 * sizeof(uint8_t);
        pixel_buffer_.resize(buffer_size);
        std::memcpy(&pixel_buffer_[0], reinterpret_cast<const void *>(upng_get_buffer(upng)), buffer_size);
    }
}
