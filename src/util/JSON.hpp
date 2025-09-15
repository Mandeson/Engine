#pragma once

#include <nlohmann/json.hpp>
using json = nlohmann::json;

namespace JSON {

json parseFileImpl(const std::string &filename);

class FileLoadingError : public std::exception {
public:
    FileLoadingError(const std::string &filename);
    const char* what() const noexcept override;
private:
    std::string message_;
};

}
