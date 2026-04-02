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
        glDrawElements(GL_TRIANGLES, render_elements_, GL_UNSIGNED_INT, 0);
    }
    void renderRange(size_t quad_offset, size_t quad_count) {
        if (quad_count == 0)
            return;
        size_t indices_count = quad_count * 2 * 3;
        size_t indices_offset = quad_offset * 2 * 3;
        assert(indices_count + indices_offset <= render_elements_);
        glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_INT,
                reinterpret_cast<void *>(indices_offset * sizeof(GLuint)));
    }
    size_t getQuadCount() {
        return vertices_.size() / 4;
    }
    static void unbind() {
        glBindVertexArray(0);
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
    bool update(GLenum usage = GL_DYNAMIC_DRAW) {
        bool bound = false;
        if (dirty_) {
            size_t indices = indices_.size();
            if (indices > allocated_indices_ || (indices != 0 && usage != last_usage_)) {
                if (OpenGL::vertexArraysSupported()) {
                    glBindVertexArray(VAO_);
                    rawBind();
                } else {
                    rawBind();
                }
                bound = true;
                glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(V),
                        reinterpret_cast<void *>(&vertices_[0]), usage);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices * sizeof(Triangle),
                        reinterpret_cast<void *>(&indices_[0]), usage);
                last_usage_ = usage;
                allocated_indices_ = indices;
            } else if (indices != 0) {
                if (OpenGL::vertexArraysSupported()) {
                    glBindVertexArray(VAO_);
                    rawBind();
                } else {
                    rawBind();
                }
                bound = true;
                glBufferSubData(GL_ARRAY_BUFFER, 0, vertices_.size() * sizeof(V),
                        reinterpret_cast<void *>(&vertices_[0]));
                glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices * sizeof(Triangle),
                        reinterpret_cast<void *>(&indices_[0]));
            }
            dirty_ = false;
            render_elements_ = indices * 3;
        }
        return bound;
    }

    bool isGenetated() {
        return generated_;
    }

    void generate() {
        glGenBuffers(1, &VBO_);
        glGenBuffers(1, &EBO_);
        if (OpenGL::vertexArraysSupported()) {
            glGenVertexArrays(1, &VAO_);
            glBindVertexArray(VAO_);
            rawBind();
        }
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
    std::vector<V> vertices_;
    std::vector<Triangle> indices_;
    GLuint indice_index_ = 0;
    size_t allocated_indices_ = 0;
    size_t render_elements_ = 0;
    GLuint VBO_;
    GLuint EBO_;
    GLenum last_usage_ = GL_DYNAMIC_DRAW;
};
