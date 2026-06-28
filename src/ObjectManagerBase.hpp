#pragma once

#include <functional>
#include <optional>
#include <vector>

// T - object class
// I - object id (usually int)
template <typename T, typename I>
class ObjectManagerBase {
public:
    template<typename... Args>
    I newObject(Args&&... __args) {
        I id = findEmptyOrCreate();
        objects_[id].emplace(std::forward<Args>(__args)...);
        return id;
    }

    void destroyObject(I object_id) {
        objects_.at(object_id).reset();
    }

    void setVisible(I object_id, bool visible) {
        objects_.at(object_id).value().visible = visible;
    }

    void forEachVisibleObject(std::function<void(T &object)> func) {
        for (auto &object : objects_)
            if (object.has_value() && (*object).visible)
                func((*object).instance);
    }
protected:
    struct Object {
        T instance;
        bool visible = true;

        template<typename... Args>
        Object(Args&&... __args) : instance(std::forward<Args>(__args)...) { }
    };

    std::vector<std::optional<Object>> objects_;
private:
    I findEmptyOrCreate() {
        for (size_t object_id = 0; object_id < objects_.size(); object_id++) {
            auto &object = objects_[object_id];
            if (!object.has_value())
                return object_id;
        }
        
        objects_.emplace_back();
        return objects_.size() - 1; // Return the last, newly added element
    }
};
