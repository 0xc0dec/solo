/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

namespace solo
{
    struct Radians;

    struct Degrees
    {
        explicit Degrees(float d): raw_(d) {}
        Degrees(const Radians &r);

        auto operator=(float f) -> Degrees&;
        auto operator=(const Degrees &d) -> Degrees& = default;
        auto operator=(const Radians &r) -> Degrees&;

        auto toRawDegrees() const -> float { return raw_; }
        auto toRawRadians() const -> float;

        auto operator+(const Degrees &d) const -> Degrees { return Degrees(raw_ + d.raw_); }
        auto operator+(const Radians &r) const -> Degrees;
        auto operator+=(const Degrees &d) -> Degrees&;
        auto operator+=(const Radians &r) -> Degrees&;
        auto operator-() const -> Degrees { return Degrees(-raw_); }
        auto operator-(const Degrees &d) const -> Degrees { return Degrees(raw_ - d.raw_); }
        auto operator-(const Radians &r) const -> Degrees;
        auto operator-=(const Degrees &d) -> Degrees&;
        auto operator-=(const Radians &r) -> Degrees&;
        auto operator*(float f) const -> Degrees { return Degrees(raw_ * f); }
        auto operator*(const Degrees &f) const -> Degrees { return Degrees(raw_ * f.raw_);}
        auto operator*=(float f) -> Degrees&;
        auto operator/(float f) const -> Degrees { return Degrees(raw_ / f); }
        auto operator/=(float f) -> Degrees&;

        bool operator<(const Degrees &d) const { return raw_ < d.raw_; }
        bool operator<=(const Degrees &d) const { return raw_ <= d.raw_; }
        bool operator>=(const Degrees &d) const { return raw_ >= d.raw_; }
        bool operator>(const Degrees &d) const { return raw_ >  d.raw_; }

    private:
        float raw_;
    };

    inline auto Degrees::operator=(float f) -> Degrees &
    {
        raw_ = f;
        return *this;
    }

    inline auto Degrees::operator+=(const Degrees &d) -> Degrees &
    {
        raw_ += d.raw_;
        return *this;
    }

    inline auto Degrees::operator-=(const Degrees &d) -> Degrees &
    {
        raw_ -= d.raw_;
        return *this;
    }

    inline auto Degrees::operator*=(float f) -> Degrees &
    {
        raw_ *= f;
        return *this;
    }

    inline auto Degrees::operator/=(float f) -> Degrees &
    {
        raw_ /= f;
        return *this;
    }
}