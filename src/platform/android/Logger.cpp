#include <android/log.h>
#include "../../util/Logger.hpp"

namespace Log {

constexpr const char *kAppName = "Engine";

void printLineInfoImpl(const std::string &str) {
    __android_log_print(ANDROID_LOG_INFO, kAppName, "%s", str.c_str());
}

void printLineErrImpl(const std::string &str) {
    __android_log_print(ANDROID_LOG_ERROR, kAppName, "%s", str.c_str());
}

void printLineWarnImpl(const std::string &str) {
    __android_log_print(ANDROID_LOG_WARN, kAppName, "%s", str.c_str());
}

void printLineDbgImpl(const std::string &str) {
    __android_log_print(ANDROID_LOG_DEBUG, kAppName, "%s", str.c_str());
}

}
