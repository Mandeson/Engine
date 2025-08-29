#include "../../../text/Font.hpp"
#include "../../../util/Logger.hpp"
#include "../asset_manager.hpp"

class Font::FileLoaderImpl {
public:
    explicit FileLoaderImpl(std::string &&path) {
        asset_ = AAssetManager_open(getAssetManager(), path.c_str(), AASSET_MODE_BUFFER);
        if (asset_ == nullptr)
            throw Font::FontLoadingError(std::move(path));
        size_ = AAsset_getLength(asset_);
        buffer_ = std::make_unique<uint8_t[]>(size_);
        if (!AAsset_read(asset_, buffer_.get(), size_))
            throw Font::FontLoadingError(std::move(path));
    }
    ~FileLoaderImpl() {
        AAsset_close(asset_);
    }
    uint8_t *getBufferData() {
        return buffer_.get();
    }
    [[nodiscard]] size_t getBufferSize() const {
        return size_;
    }
private:
    AAsset *asset_;
    size_t size_;
    std::unique_ptr<uint8_t[]> buffer_;
};

Font::Font(const char *filename) {
    Font::add();
    std::string path = std::string("fonts/") + filename;
    file_loader_impl_ = std::make_unique<Font::FileLoaderImpl>(std::move(path));
    if (FT_New_Memory_Face(library_, file_loader_impl_->getBufferData(),
                           static_cast<FT_Long>(file_loader_impl_->getBufferSize()), 0, &face_) != 0)
		throw FontLoadingError(std::move(path));
    Log::dbg("Created font face");
}

Font::~Font() {
    FT_Done_Face(face_);
    Log::dbg("Destroyed font face");
    Font::remove();
}
