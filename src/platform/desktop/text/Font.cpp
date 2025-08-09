#include "../../../text/Font.hpp"
#include "../../../util/Logger.hpp"

class Font::FileLoaderImpl { }; // implementation-specific data not needed

Font::Font(const char *filename) {
    Font::add();
    std::string path = std::string("assets/fonts/") + filename;
    if (FT_New_Face(library_, path.c_str(), 0, &face_) != 0)
		throw FontLoadingError(std::move(path));
    Log::dbg("Created font face");
}

Font::~Font() {
    FT_Done_Face(face_);
    Log::dbg("Destroyed font face");
    Font::remove();
}
