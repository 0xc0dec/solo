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
        explicit Degrees(float d): raw(d) {}
        Degrees(const Radians &r);

        auto operator=(float f) -> Degrees&;
        auto operator=(const Degrees &d) -> Degrees& = default;
        auto operator=(const Radians &r) -> Degrees&;

        auto toRawDegrees() const -> float { return raw; }
        auto toRawRadians() const -> float;

        auto operator+(const Degrees &d) const -> Degrees { return Degrees(raw + d.raw); }
        auto operator+(const Radians &r) const -> Degrees;
        auto operator+=(const Degrees &d) -> Degrees&;
        auto operator+=(const Radians &r) -> Degrees&;
        auto operator-() const -> Degrees { return Degrees(-raw); }
        auto operator-(const Degrees &d) const -> Degrees { return Degrees(raw - d.raw); }
        auto operator-(const Radians &r) const -> Degrees;
        auto operator-=(const Degrees &d) -> Degrees&;
        auto operator-=(const Radians &r) -> Degrees&;
        auto operator*(float f) const -> Degrees { return Degrees(raw * f); }
        auto operator*(const Degrees &f) const -> Degrees { return Degrees(raw * f.raw);}
        auto operator*=(float f) -> Degrees&;
        auto operator/(float f) const -> Degrees { return Degrees(raw / f); }
        auto operator/=(float f) -> Degrees&;

        bool operator<(const Degrees &d) const { return raw < d.raw; }
        bool operator<=(const Degrees &d) const { return raw <= d.raw; }
        bool operator>=(const Degrees &d) const { return raw >= d.raw; }
        bool operator>(const Degrees &d) const { return raw >  d.raw; }

    private:
        float raw;
    };

    inline auto Degrees::operator=(float f) -> Degrees &
    {
        raw = f;
        return *this;
    }

    inline auto Degrees::operator+=(const Degrees &d) -> Degrees &
    {
        raw += d.raw;
        return *this;
    }

    inline auto Degrees::operator-=(const Degrees &d) -> Degrees &
    {
        raw -= d.raw;
        return *this;
    }

    inline auto Degrees::operator*=(float f) -> Degrees &
    {
        raw *= f;
        return *this;
    }

    inline auto Degrees::operator/=(float f) -> Degrees &
    {
        raw /= f;
        return *this;
    }
}