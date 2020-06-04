/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#pragma once

#include "SoloRadians.h"
#include "SoloMath.h"

namespace solo
{
    class Vector2 final
    {
    public:
        Vector2() = default;
        Vector2(const glm::vec2 &data);
        Vector2(float all);
        Vector2(float x, float y);

        auto x() -> float& { return data_.x; }
        auto x() const -> float { return data_.x; }
        auto y() -> float& { return data_.y; }
        auto y() const -> float { return data_.y; }

        bool isZero() const;
        bool isUnit() const;

        auto distance(const Vector2 &v) const -> float;
        auto length() const -> float;
        auto normalized() const -> Vector2;
        void normalize();
        auto angle(const Vector2 &v) const -> Radians;
        void clamp(const Vector2 &min, const Vector2 &max);
        auto dot(const Vector2 &v) const -> float;

        auto operator=(const glm::vec2 &other) -> Vector2&;

        bool operator==(const Vector2 &v) const;
        bool operator!=(const Vector2 &v) const;

        auto operator+(const Vector2 &v) const -> Vector2;
        auto operator+=(float scalar) -> Vector2&;
        auto operator+=(const Vector2 &v) -> Vector2&;

        auto operator-() const -> Vector2;
        auto operator-(const Vector2 &v) const -> Vector2;
        auto operator-=(float scalar) -> Vector2&;
        auto operator-=(const Vector2 &v) -> Vector2&;

        auto operator*(float scalar) const -> Vector2;
        auto operator*=(float scalar) -> Vector2&;

        auto operator/(float scalar) const -> Vector2;
        auto operator/=(float scalar) -> Vector2&;

    private:
        glm::vec2 data_;
    };
}
