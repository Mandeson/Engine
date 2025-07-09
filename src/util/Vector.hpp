#pragma once

template <typename T>
struct Vector2 {
    T x;
    T y;

    template <typename D>
    Vector2<T> operator*(D val) const {
        return Vector2<T>{static_cast<T>(x * val), static_cast<T>(y * val)};
    }
};

using Vector2i = Vector2<int>;
using Vector2f = Vector2<float>;
