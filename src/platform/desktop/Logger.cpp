#include <iostream>
#include "../../util/Logger.hpp"

void Log::printLineInfoImpl(const std::string &str) {
    std::cout << "[Info] " + str + "\n";
}

void Log::printLineErrImpl(const std::string &str) {
    std::cerr << "[Error] " + str + "\n";
    std::cerr.flush();
}

void Log::printLineWarnImpl(const std::string &str) {
    std::cerr << "[Warn] " + str + "\n";
    std::cerr.flush();
}

void Log::printLineDbgImpl(const std::string &str) {
    std::cout << "[Debug] " + str + "\n";
}
