#pragma once

#include <cstdint>
#include <string>
#include <lua.hpp>
#include <upng/upng.h>
#include "OpenGL.hpp"
#include "util/Vector.hpp"
#include "PipelineState.hpp"
#include "ThreadPool.hpp"

class Texture {
public:
    enum class Filtering {
        kMipmapLinear,
        kLinear,
        kNearest,
    };

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

    Texture(ThreadPool &thread_pool, const std::string &filename, Filtering filtering = Filtering::kMipmapLinear);

    Texture(const Texture &) = delete;
    Texture& operator=(const Texture &) = delete;

    ~Texture();
    bool ready();
    Vector2i getSize();
    void bind(PipelineState &pipeline_state);
private:
    void loadImpl(const std::string &filename);
    void decode(std::string &&filename, upng_t *upng);

    Filtering filtering_;
    bool background_work_active_;
    std::mutex mutex_;
    std::condition_variable background_work_condition_;
    std::atomic<bool> error_ = false;
    std::exception_ptr exception_ = nullptr;
    std::atomic<bool> ready_ = false;
    bool alpha_ = false;
    Vector2i size_;
    std::vector<uint8_t> pixel_buffer_;
    GLuint texture_id_;
    bool uploaded_ = false;
};
