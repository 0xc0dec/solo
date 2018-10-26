/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloRadians.h"
#include "SoloMath.h"

namespace solo
{
    class Vector4 final
    {
    public:
        Vector4() = default;
        Vector4(const glm::vec4 &data);
        Vector4(float all);
        Vector4(float x, float y, float z, float w);

        auto x() -> float& { return data_.x; }
        auto x() const -> float { return data_.x; }
        auto y() -> float& { return data_.y; }
        auto y() const -> float { return data_.y; }
        auto z() -> float& { return data_.z; }
        auto z() const -> float { return data_.z; }
        auto w() -> float& { return data_.w; }
        auto w() const -> float { return data_.w; }

        bool isZero() const;
        bool isUnit() const;

        auto distance(const Vector4 &v) const -> float;
        auto length() const -> float;
        auto normalized() const -> Vector4;
        void normalize();
        auto angle(const Vector4 &v) -> Radians;
        void clamp(const Vector4 &min, const Vector4 &max);
        auto dot(const Vector4 &v) const -> float;

        auto operator=(const glm::vec4 &other) -> Vector4&;

        bool operator==(const Vector4 &v) const;
        bool operator!=(const Vector4 &v) const;

        auto operator+(float scalar) const -> Vector4;
        auto operator+(const Vector4 &v) const -> Vector4;
        auto operator+=(float scalar) -> Vector4&;
        auto operator+=(const Vector4 &v) -> Vector4&;

        auto operator-() const -> Vector4;
        auto operator-(float scalar) const -> Vector4;
        auto operator-(const Vector4 &v) const -> Vector4;
        auto operator-=(float scalar)  -> Vector4&;
        auto operator-=(const Vector4 &v) -> Vector4&;

        auto operator*(float x) const -> Vector4;
        auto operator*=(float x) -> Vector4&;

        auto operator/(float x) const -> Vector4;
        auto operator/=(float x) -> Vector4&;

    private:
        glm::vec4 data_;
    };
}
