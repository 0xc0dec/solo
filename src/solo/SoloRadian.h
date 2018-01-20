/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

namespace solo
{
    struct Degree;

    struct Radian
    {
        explicit Radian(float raw);
        Radian(const Degree &d);

        auto operator=(const float &f) -> Radian&;
        auto operator=(const Radian &r) -> Radian&;
        auto operator=(const Degree &d) -> Radian&;

        auto toRawDegree() const -> float;
        auto toRawRadian() const -> float;

        auto operator+(const Radian &r) const -> Radian;
        auto operator+(const Degree &d) const -> Radian;
        auto operator+=(const Radian &r) -> Radian&;
        auto operator+=(const Degree &d) -> Radian&;
        auto operator-() const -> Radian;
        auto operator-(const Radian &r) const -> Radian;
        auto operator-(const Degree &d) const -> Radian;
        auto operator-=(const Radian &r) -> Radian&;
        auto operator-=(const Degree &d) -> Radian&;
        auto operator*(float f) const -> Radian;
        auto operator*=(float f) -> Radian&;
        auto operator/(float f) const -> Radian;
        auto operator/=(float f) -> Radian&;

        bool operator<(const Radian &r) const;
        bool operator<=(const Radian &r) const;
        bool operator>=(const Radian &r) const;
        bool operator>(const Radian &r) const;

    private:
        float raw;
    };

    inline Radian::Radian(float raw): raw(raw)
    {
    }

    inline auto Radian::operator=(const float &f) -> Radian &
    {
        raw = f;
        return *this;
    }

    inline auto Radian::operator=(const Radian &r) -> Radian &
    {
        raw = r.raw;
        return *this;
    }

    inline auto Radian::toRawRadian() const -> float
    {
        return raw;
    }

    inline auto Radian::operator+(const Radian &r) const -> Radian
    {
        return Radian(raw + r.raw);
    }

    inline auto Radian::operator+=(const Radian &r) -> Radian &
    {
        raw += r.raw;
        return *this;
    }

    inline auto Radian::operator-() const -> Radian
    {
        return Radian(-raw);
    }

    inline auto Radian::operator-(const Radian &r) const -> Radian
    {
        return Radian(raw - r.raw);
    }

    inline auto Radian::operator-=(const Radian &r) -> Radian &
    {
        raw -= r.raw;
        return *this;
    }

    inline auto Radian::operator*(float f) const -> Radian
    {
        return Radian(raw * f);
    }

    inline auto Radian::operator*=(float f) -> Radian &
    {
        raw *= f;
        return *this;
    }

    inline auto Radian::operator/(float f) const -> Radian
    {
        return Radian(raw / f);
    }

    inline auto Radian::operator/=(float f) -> Radian &
    {
        raw /= f;
        return *this;
    }

    inline bool Radian::operator<(const Radian &r) const
    {
        return raw <  r.raw;
    }

    inline bool Radian::operator<=(const Radian &r) const
    {
        return raw <= r.raw;
    }

    inline bool Radian::operator>=(const Radian &r) const
    {
        return raw >= r.raw;
    }

    inline bool Radian::operator>(const Radian &r) const
    {
        return raw > r.raw;
    }
}