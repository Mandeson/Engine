#pragma once

#include "ThreadPool.hpp"
#include <string>
#include <lua.hpp>
#include <upng/upng.h>
#include "OpenGL.hpp"
#include "util/Vector.hpp"

class Texture {
public:
    class FileNotFoundError : public std::exception {
    public:
        FileNotFoundError(std::string &&filename);
        const char* what() const noexcept override;
    private:
        std::string message_;
    };

    class DecodingError : public std::exception {
    public:
        DecodingError(std::string &&filename);
        const char* what() const noexcept override;
    private:
        std::string message_;
    };

    Texture(ThreadPool &thread_pool, const std::string &filename);

    Texture(const Texture &) = delete;
    Texture& operator=(const Texture &) = delete;

    ~Texture();
    bool ready();
    void bind();
private:
    void loadImpl(const std::string &filename);
    void decode(std::string &&filename, upng_t *upng);

    bool background_work_active_;
    std::mutex mutex_;
    std::condition_variable background_work_condition_;
    std::atomic<bool> error_ = false;
    std::exception_ptr exception_ = nullptr;
    std::atomic<bool> ready_ = false;
    bool alpha_;
    Vector2i size_;
    std::vector<uint8_t> pixel_buffer_;
    GLuint texture_id_;
    bool uploaded_ = false;
};
