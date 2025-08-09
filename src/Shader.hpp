#pragma once

#include <string>
#include <exception>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "OpenGL.hpp"
#include "util/Vector.hpp"
#include "util/Color.hpp"

class Shader {
public:
    class FileNotFoundError : public std::exception {
    public:
        FileNotFoundError(std::string&& filename);
        const char* what() const noexcept override;
    private:
        std::string message_;
    };

    class CompileError : public std::exception {
    public:
        CompileError(std::string&& filename, std::string&& compile_error);
        const char* what() const noexcept override;
    private:
        std::string message_;
    };

    class LinkError : public std::exception {
    public:
        LinkError(std::string&& name, std::string&& link_error);
        const char* what() const noexcept override;
        const std::string& getName();
        const std::string& getLinkError();
    private:
        std::string name_;
        std::string link_error_;
    };

    class NotLoaded : public std::exception {
    public:
        const char* what() const noexcept override;
    };

    /* Load the shader from disk and compile with OpenGL
     * Must be called from the opengl context thread
     */
    Shader(const std::string &name);
    Shader(const Shader &) = delete;
    Shader& operator=(const Shader &) = delete;
    ~Shader();

    void use();
    GLuint getAttribLocation(const char* name);
    GLuint getUniformLocation(const char* name);
    static void setUniformMat4(GLuint uniform_location, glm::mat4 matrix);
    static void setUniform1f(GLuint uniform_location, float val);
    static void setUniform2f(GLuint uniform_location, Vector2f vec);
    // Set a vec3 uniform to normalized color data
    static void setUniformColor(GLuint uniform_location, Color color);
private:
    static void loadSourceImpl(GLuint shader, std::string &filename);
    static GLuint compile(std::string &&filename, GLenum type);

    GLuint id_{};
};
