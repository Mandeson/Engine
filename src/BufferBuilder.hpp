#pragma once

#include "OpenGL.hpp"
#include "util/Logger.hpp"
#include <array>
#include <cassert>
#include <vector>

template<typename V> // Vertex type
class BufferBuilder {
public:
    virtual ~BufferBuilder() {
        if (generated_) {
            Log::dbg("Destroying buffer");
            glDeleteBuffers(1, &VBO_);
            glDeleteBuffers(1, &EBO_);
        }
    }
    void clear() {
        vertices_.clear();
        indices_.clear();
        indice_index_ = 0;
    }
    void end() {
        dirty_ = true;
    }
    void render() {
        glDrawElements(GL_TRIANGLES, indices_.size() * 3, // A triangle has 3 vertices
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
    // Call from render thread
    bool bind(GLenum usage = GL_DYNAMIC_DRAW) {
        if (dirty_) {
            if (indice_index_ == 0) { // buffer empty
                render_ready_ = false;
                dirty_ = false;
                Log::dbg("Buffer empty");
                return render_ready_;
            }
            Log::dbg("Flushing buffer");
            size_t indices = indices_.size();
            if (indices > allocated_indices_) {
                if (!generated_) {
                    glGenBuffers(1, &VBO_);
                    glGenBuffers(1, &EBO_);
                    generated_ = true;
                }
                glBindBuffer(GL_ARRAY_BUFFER, VBO_);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
                glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(V),
                        reinterpret_cast<void *>(&vertices_[0]), usage);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices * sizeof(Triangle),
                        reinterpret_cast<void *>(&indices_[0]), usage);
                allocated_indices_ = indices;
            } else if (indices != 0) {
                assert(generated_);
                glBindBuffer(GL_ARRAY_BUFFER, VBO_);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
                glBufferSubData(GL_ARRAY_BUFFER, 0, vertices_.size() * sizeof(V),
                        reinterpret_cast<void *>(&vertices_[0]));
                glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices * sizeof(Triangle),
                        reinterpret_cast<void *>(&indices_[0]));
            }
            dirty_ = false;
            render_ready_ = true;
        } else if (generated_ && render_ready_) {
            glBindBuffer(GL_ARRAY_BUFFER, VBO_);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
        }
        return render_ready_;
    }
private:
    struct Triangle {
        GLuint a;
        GLuint b;
        GLuint c;
    };

    bool generated_;
    bool dirty_;
    bool render_ready_;
    std::vector<V> vertices_;
    std::vector<Triangle> indices_;
    unsigned int indice_index_ = 0;
    size_t allocated_indices_ = 0;
    GLuint VBO_;
    GLuint EBO_;
};
