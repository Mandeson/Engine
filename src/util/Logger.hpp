#pragma once

#include <format>
#include <iostream>
#include <string_view>

namespace Log {

void printLineInfoImpl(const std::string &str);
void printLineErrImpl(const std::string &str);
void printLineWarnImpl(const std::string &str);
void printLineDbgImpl(const std::string &str);

void info(const std::string &str);
void err(const std::string &str);
void warn(const std::string &str);
void dbg(const std::string &str);

template<typename... Args>
void info(std::string_view str, Args&&... args) {
    printLineInfoImpl(std::vformat(str, std::make_format_args(args...)));
}

template<typename... Args>
void err(std::string_view str, Args&&... args) {
    printLineErrImpl(std::vformat(str, std::make_format_args(args...)));
}

template<typename... Args>
void warn(std::string_view str, Args&&... args) {
    printLineWarnImpl(std::vformat(str, std::make_format_args(args...)));
}

template<typename... Args>
void dbg([[maybe_unused]] std::string_view str, [[maybe_unused]] Args&&... args) {
#ifdef ENGINE_DEBUG
    printLineDbgImpl(std::vformat(str, std::make_format_args(args...)));
#endif
}

}
