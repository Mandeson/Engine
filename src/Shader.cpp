#include <format>
#include "Shader.hpp"

Shader::FileNotFoundError::FileNotFoundError(std::string &&filename)
        : message_(std::format("Shader file not found: {}", filename)) { }

const char* Shader::FileNotFoundError::what() const noexcept {
    return message_.c_str();
}

Shader::CompileError::CompileError(std::string &&filename, std::string &&compile_error)
        : message_(std::format("Shader compilation error: {}\n{}", filename, compile_error)) { }

const char* Shader::CompileError::what() const noexcept {
    return message_.c_str();
}

Shader::LinkError::LinkError(std::string &&name, std::string &&link_error)
        : name_(name), link_error_(link_error) { }

const char* Shader::LinkError::what() const noexcept {
    return "Shader: link error";
}

const std::string& Shader::LinkError::getName() {
    return name_;
}

const std::string& Shader::LinkError::getLinkError() {
    return link_error_;
}

const char* Shader::NotLoaded::what() const noexcept {
    return "Shader: using a shader that is not loaded";
}

GLuint Shader::compile(std::string &&filename, GLenum type)
{
    GLuint shader = glCreateShader(type);
    if (shader == 0)
        throw std::runtime_error("Could not create an OpenGL Shader object");
    Shader::loadSourceImpl(shader, filename);
    glCompileShader(shader);
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLsizei error_msg_size;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &error_msg_size);
        std::string error_msg(error_msg_size, '\0');
        glGetShaderInfoLog(shader, error_msg_size, nullptr, &error_msg[0]);
        throw Shader::CompileError(std::move(filename), std::move(error_msg));
    }
    return shader;
}

Shader::Shader(const std::string &name) {
    id_ = glCreateProgram();
    if (id_ == 0)
        throw std::runtime_error("Could not create an OpenGL Program object");

#ifdef USE_GLES2
    std::string path = "shaders/GLES/";
#else
    std::string path = "shaders/GL/";
#endif

    GLuint vertex = Shader::compile(path + name + ".vs", GL_VERTEX_SHADER);
    glAttachShader(id_, vertex);
    glDeleteShader(vertex);

    GLuint fragment = Shader::compile(path + name + ".fs", GL_FRAGMENT_SHADER);
    glAttachShader(id_, fragment);
    glDeleteShader(fragment);

    glLinkProgram(id_);
    GLint status;
    glGetProgramiv(id_, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        GLsizei error_msg_size;
        glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &error_msg_size);
        std::string error_msg(error_msg_size, '\0');
        glGetProgramInfoLog(id_, error_msg_size, nullptr, &error_msg[0]);
        throw Shader::LinkError(std::string(name), std::move(error_msg));
    }
}

Shader::~Shader()
{
    if (id_)
        glDeleteProgram(id_);
}

void Shader::use() {
    if (id_ == 0)
        throw Shader::NotLoaded();
    glUseProgram(id_);
}

GLuint Shader::getAttribLocation(const char* name) {
    if (id_ == 0)
        throw Shader::NotLoaded();
    return glGetAttribLocation(id_, name);
}

GLuint Shader::getUniformLocation(const char *name) {
    if (id_ == 0)
        throw Shader::NotLoaded();
    return glGetUniformLocation(id_, name);
}

void Shader::setUniformMat4(GLuint uniform_location, glm::mat4 matrix) {
    glUniformMatrix4fv(uniform_location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setUniform1f(GLuint uniform_location, float val) {
    glUniform1f(uniform_location, val);
}

void Shader::setUniform2f(GLuint uniform_location, Vector2f vec) {
    glUniform2f(uniform_location, vec.x, vec.y);
}

void Shader::setUniformColor(GLuint uniform_location, Color color) {
    float r = color.r * (1 / 255.0f);
    float g = color.g * (1 / 255.0f);
    float b = color.b * (1 / 255.0f);
    glUniform3f(uniform_location, r, g, b);
}
