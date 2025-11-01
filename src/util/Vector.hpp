#pragma once

template <typename T>
struct Vector2 {
    T x;
    T y;

    template <typename D>
    Vector2<T> operator*(D val) const {
        return Vector2<T>{static_cast<T>(x * val), static_cast<T>(y * val)};
    }

    template <typename D>
    Vector2<T> operator/(D val) const {
        return Vector2<T>{static_cast<T>(x / val), static_cast<T>(y / val)};
    }
    
    Vector2<T> operator+(const Vector2<T> &other) const {
        return Vector2<T>{static_cast<T>(x + other.x), static_cast<T>(y + other.y)};
    }

    Vector2<T> operator-(const Vector2<T> &other) const {
        return Vector2<T>{static_cast<T>(x - other.x), static_cast<T>(y - other.y)};
    }

    Vector2<T> operator-() const {
        return Vector2<T>{-x, -y};
    }

    template <typename S>
    operator Vector2<S>() const {
        return Vector2<S>{static_cast<S>(x), static_cast<S>(y)};
    }
};

using Vector2i = Vector2<int>;
using Vector2f = Vector2<float>;
using Vector2d = Vector2<double>;
