/* 
 * Copyright (c) Aleksey Fedotov 
 * MIT license 
 */

#pragma once

#include "SoloRadians.h"
#include "SoloMath.h"
#include <glm/vec3.hpp>

namespace solo
{
    class Vector3 final
    {
    public:
        Vector3() = default;
        Vector3(const glm::vec3 &data);
        Vector3(float all);
        Vector3(float x, float y, float z);

        auto x() -> float& { return data_.x; }
        auto x() const -> float { return data_.x; }
        auto y() -> float& { return data_.y; }
        auto y() const -> float { return data_.y; }
        auto z() -> float& { return data_.z; }
        auto z() const -> float { return data_.z; }

        bool isZero() const;
        bool isUnit() const;

        auto distance(const Vector3 &v) const -> float;
        auto length() const -> float;
        auto normalized() const -> Vector3;
        void normalize();
        auto angle(const Vector3 &v) const -> Radians;
        void clamp(const Vector3 &min, const Vector3 &max);
        auto dot(const Vector3 &v) const -> float;
        auto cross(const Vector3 &v) const -> Vector3;

        operator glm::vec3() const { return data_; }

        auto operator=(const glm::vec3 &other) -> Vector3&;

        bool operator==(const Vector3 &v) const;
        bool operator!=(const Vector3 &v) const;

        auto operator+(float scalar) const -> Vector3;
        auto operator+(const Vector3 &v) const -> Vector3;
        auto operator+=(const Vector3 &v) -> Vector3&;
        auto operator+=(float scalar) -> Vector3&;

        auto operator-() const -> Vector3;
        auto operator-(float scalar) const -> Vector3;
        auto operator-(const Vector3 &v) const -> Vector3;
        auto operator-=(float scalar) -> Vector3&;
        auto operator-=(const Vector3 &v) -> Vector3&;

        auto operator*(float scalar) const -> Vector3;
        auto operator*=(float scalar) -> Vector3&;

        auto operator/(float scalar) const -> Vector3;
        auto operator/=(float scalar) -> Vector3&;

    private:
        glm::vec3 data_;
    };
}
