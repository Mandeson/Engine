#include "../../Shader.hpp"
#include <fstream>
#include <filesystem>

void Shader::loadSourceImpl(GLuint shader, std::string &filename) {
    std::string path = "assets/" + filename;
    try {
        auto size = std::filesystem::file_size(path);
        std::string text(size, '\0');
        std::ifstream in(path);
        if (in.fail())
            throw Shader::FileNotFoundError(std::move(path));
        in.read(&text[0], size);
        const GLchar *text_c_str = text.c_str();
        glShaderSource(shader, 1, &text_c_str, 0);
    } catch (std::filesystem::filesystem_error& e) {
        throw Shader::FileNotFoundError(std::move(path));
    }
    
}
