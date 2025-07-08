#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "util/Logger.hpp"

int main() {
    Log::err("Hello {}, {}", 4, 4.3);
}
