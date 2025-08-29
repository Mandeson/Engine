#include <iostream>
#include "../../util/Logger.hpp"

namespace Log {

    constexpr const char *kAppName = "Engine";

    void printLineInfoImpl(const std::string &str) {
        std::cout << "[Info] " + str + "\n";
    }

    void printLineErrImpl(const std::string &str) {
        std::cerr << "[Error] " + str + "\n";
        std::cerr.flush();
    }

    void printLineWarnImpl(const std::string &str) {
        std::cerr << "[Warn] " + str + "\n";
        std::cerr.flush();
    }

    void printLineDbgImpl(const std::string &str) {
        std::cout << "[Debug] " + str + "\n";
    }

}
