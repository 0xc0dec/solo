/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

namespace solo
{
    struct Radian;

    struct Degree
    {
        explicit Degree(float d);
        Degree(const Radian &r);

        auto operator=(float f) -> Degree&;
        auto operator=(const Degree &d) -> Degree&;
        auto operator=(const Radian &r) -> Degree&;

        auto toRawDegree() const -> float;
        auto toRawRadian() const -> float;

        auto operator+(const Degree &d) const -> Degree;
        auto operator+(const Radian &r) const -> Degree;
        auto operator+=(const Degree &d) -> Degree&;
        auto operator+=(const Radian &r) -> Degree&;
        auto operator-() const -> Degree;
        auto operator-(const Degree &d) const -> Degree;
        auto operator-(const Radian &r) const -> Degree;
        auto operator-=(const Degree &d) -> Degree&;
        auto operator-=(const Radian &r) -> Degree&;
        auto operator*(float f) const -> Degree;
        auto operator*(const Degree &f) const -> Degree;
        auto operator*=(float f) -> Degree&;
        auto operator/(float f) const -> Degree;
        auto operator/=(float f) -> Degree&;

        bool operator<(const Degree &d) const;
        bool operator<=(const Degree &d) const;
        bool operator>=(const Degree &d) const;
        bool operator>(const Degree &d) const;

    private:
        float raw;
    };

    inline Degree::Degree(float d): raw(d)
    {
    }

    inline auto Degree::operator=(float f) -> Degree &
    {
        raw = f;
        return *this;
    }

    inline auto Degree::operator=(const Degree &d) -> Degree &
    {
        raw = d.raw;
        return *this;
    }

    inline auto Degree::toRawDegree() const -> float
    {
        return raw;
    }

    inline auto Degree::operator+(const Degree &d) const -> Degree
    {
        return Degree(raw + d.raw);
    }

    inline auto Degree::operator+=(const Degree &d) -> Degree &
    {
        raw += d.raw;
        return *this;
    }

    inline auto Degree::operator-() const -> Degree
    {
        return Degree(-raw);
    }

    inline auto Degree::operator-(const Degree &d) const -> Degree
    {
        return Degree(raw - d.raw);
    }

    inline auto Degree::operator-=(const Degree &d) -> Degree &
    {
        raw -= d.raw;
        return *this;
    }

    inline auto Degree::operator*(float f) const -> Degree
    {
        return Degree(raw * f);
    }

    inline auto Degree::operator*(const Degree &f) const -> Degree
    {
        return Degree(raw * f.raw);
    }

    inline auto Degree::operator*=(float f) -> Degree &
    {
        raw *= f;
        return *this;
    }

    inline auto Degree::operator/(float f) const -> Degree
    {
        return Degree(raw / f);
    }

    inline auto Degree::operator/=(float f) -> Degree &
    {
        raw /= f;
        return *this;
    }

    inline bool Degree::operator<(const Degree &d) const
    {
        return raw < d.raw;
    }

    inline bool Degree::operator<=(const Degree &d) const
    {
        return raw <= d.raw;
    }

    inline bool Degree::operator>=(const Degree &d) const
    {
        return raw >= d.raw;
    }

    inline bool Degree::operator>(const Degree &d) const
    {
        return raw >  d.raw;
    }
}