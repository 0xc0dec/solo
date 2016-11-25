/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
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
        auto operator+(const Radian &r) -> Degree;
        auto operator+=(const Degree &d) -> Degree&;
        auto operator+=(const Radian &r) -> Degree&;
        auto operator-() const -> Degree;
        auto operator-(const Degree &d) const -> Degree;
        auto operator-(const Radian &r) -> Degree;
        auto operator-=(const Degree &d) -> Degree&;
        auto operator-=(const Radian &r) -> Degree&;
        auto operator*(float f) const -> Degree;
        auto operator*(const Degree &f) const -> Degree;
        auto operator*=(float f) -> Degree&;
        auto operator/(float f) const -> Degree;
        auto operator/=(float f) -> Degree&;

        bool operator<(const Degree &d) const;
        bool operator<=(const Degree &d) const;
        bool operator==(const Degree &d) const;
        bool operator!=(const Degree &d) const;
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

    inline bool Degree::operator==(const Degree &d) const
    {
        return raw == d.raw;
    }

    inline bool Degree::operator!=(const Degree &d) const
    {
        return raw != d.raw;
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