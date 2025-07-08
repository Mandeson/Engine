#pragma once

#include <format>
#include <iostream>
#include <string_view>

namespace Log {
    template<typename... Args>
    void info(std::string_view str, Args&&... args) {
        std::cout << "[Info] " << std::vformat(str, std::make_format_args(args...)) << std::endl;
    }

    template<typename... Args>
    void err(std::string_view str, Args&&... args) {
        std::cerr << "[Error] " << std::vformat(str, std::make_format_args(args...)) << std::endl;
    }

    template<typename... Args>
    void warn(std::string_view str, Args&&... args) {
        std::cerr << "[Warning] "<<  std::vformat(str, std::make_format_args(args...)) << std::endl;
    }

    template<typename... Args>
    void dbg(std::string_view str, Args&&... args) {
        std::cout << "[Debug] " << std::vformat(str, std::make_format_args(args...)) << std::endl;
    }
}
