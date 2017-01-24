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
        auto operator*(const Radian &f) const -> Radian;
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

    inline auto Radian::operator*(const Radian &f) const -> Radian
    {
        return Radian(raw * f.raw);
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