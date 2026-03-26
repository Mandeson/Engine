#pragma once

#include "OpenGL.hpp"
#include "util/Logger.hpp"
#include <array>
#include <cassert>
#include <vector>

template<typename V> // Vertex type
class BufferBuilder {
public:
    BufferBuilder() = default;

    virtual ~BufferBuilder() {
        if (generated_) {
            Log::dbg("Destroying buffer");
            glDeleteBuffers(1, &VBO_);
            glDeleteBuffers(1, &EBO_);
            if (OpenGL::vertexArraysSupported())
                glDeleteVertexArrays(1, &VAO_);
        }
    }
    void clear() {
        vertices_.clear();
        indices_.clear();
        indice_index_ = 0;
        dirty_ = false;
    }
    void end() {
        dirty_ = true;
    }
    void render() {
        glDrawElements(GL_TRIANGLES, render_elements_, // A triangle has 3 vertices
            GL_UNSIGNED_INT, 0);
    }
protected:
    void addQuad(const std::array<V, 4> &vertices) {
        for (const V &vertice : vertices)
            vertices_.push_back(vertice);

        indices_.emplace_back(Triangle{indice_index_, indice_index_ + 1, indice_index_ + 2});
        indices_.emplace_back(Triangle{indice_index_ + 2, indice_index_ + 3, indice_index_});
        indice_index_ += 4;
    }

    bool empty() {
        return render_elements_ == 0;
    }

    void rawBind() {
        glBindBuffer(GL_ARRAY_BUFFER, VBO_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
    }

    // Returns true if the buffer was bound during update
    bool update(bool bound, GLenum usage = GL_DYNAMIC_DRAW) {
        if (dirty_) {
            Log::dbg("Flushing buffer");
            size_t indices = indices_.size();
            if (indices > allocated_indices_) {
                if (!bound) {
                    rawBind();
                    bound = true;
                }
                glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(V),
                        reinterpret_cast<void *>(&vertices_[0]), usage);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices * sizeof(Triangle),
                        reinterpret_cast<void *>(&indices_[0]), usage);
                allocated_indices_ = indices;
            } else if (indices != 0) {
                if (!bound) {
                    rawBind();
                    bound = true;
                }
                glBufferSubData(GL_ARRAY_BUFFER, 0, vertices_.size() * sizeof(V),
                        reinterpret_cast<void *>(&vertices_[0]));
                glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices * sizeof(Triangle),
                        reinterpret_cast<void *>(&indices_[0]));
            }
            dirty_ = false;
            render_elements_ = indices * 3;
            render_ready_ = true;
        }

        return bound;
    }

    bool isGenetated() {
        return generated_;
    }

    void generate() {
        if (OpenGL::vertexArraysSupported())
            glGenVertexArrays(1, &VAO_);
        glGenBuffers(1, &VBO_);
        glGenBuffers(1, &EBO_);
        generated_ = true;
    }

    GLuint VAO_;
private:
    struct Triangle {
        GLuint a;
        GLuint b;
        GLuint c;
    };

    bool generated_ = false;
    bool dirty_ = false;
    bool render_ready_ = false;
    std::vector<V> vertices_;
    std::vector<Triangle> indices_;
    GLuint indice_index_ = 0;
    size_t allocated_indices_ = 0;
    size_t render_elements_ = 0;
    GLuint VBO_;
    GLuint EBO_;
};
