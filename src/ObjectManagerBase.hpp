#pragma once

#include <functional>
#include <optional>
#include <vector>

// T - object class
// I - object id (usually int)
template <typename T, typename I>
class ObjectManagerBase {
public:
    void destroyObject(I object_id) {
        objects_.at(object_id).reset();
    }

    void forEachObject(std::function<void(T &object_id)> func) {
        for (auto &object : objects_)
            if (object.has_value())
                func(*object);
    }
protected:
    I findEmptyOrCreate() {
        for (size_t object_id = 0; object_id < objects_.size(); object_id++) {
            auto &object = objects_[object_id];
            if (!object.has_value())
                return object_id;
        }
        
        objects_.emplace_back();
        return objects_.size() - 1; // Return the last, newly added element
    }

    std::vector<std::optional<T>> objects_;
};
