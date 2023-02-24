/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#pragma once

namespace solo {
    struct Degrees;

    struct Radians {
        explicit Radians(float raw): raw_(raw) {}
        Radians(const Degrees &d);

        auto operator=(const float &f) -> Radians&;
        auto operator=(const Radians &r) -> Radians & = default;
        auto operator=(const Degrees &d) -> Radians&;

        auto toRawDegrees() const -> float;
        auto toRawRadians() const -> float {
            return raw_;
        }

        auto operator+(const Radians &r) const -> Radians {
            return Radians(raw_ + r.raw_);
        }
        auto operator+(const Degrees &d) const -> Radians;
        auto operator+=(const Radians &r) -> Radians&;
        auto operator+=(const Degrees &d) -> Radians&;
        auto operator-() const -> Radians {
            return Radians(-raw_);
        }
        auto operator-(const Radians &r) const -> Radians {
            return Radians(raw_ - r.raw_);
        }
        auto operator-(const Degrees &d) const -> Radians;
        auto operator-=(const Radians &r) -> Radians&;
        auto operator-=(const Degrees &d) -> Radians&;
        auto operator*(float f) const -> Radians {
            return Radians(raw_ * f);
        }
        auto operator*(const Radians &f) const -> Radians {
            return Radians(raw_ * f.raw_);
        }
        auto operator*=(float f) -> Radians&;
        auto operator/(float f) const -> Radians {
            return Radians(raw_ / f);
        }
        auto operator/=(float f) -> Radians&;

        bool operator<(const Radians &r) const {
            return raw_ <  r.raw_;
        }
        bool operator<=(const Radians &r) const {
            return raw_ <= r.raw_;
        }
        bool operator>=(const Radians &r) const {
            return raw_ >= r.raw_;
        }
        bool operator>(const Radians &r) const {
            return raw_ > r.raw_;
        }

    private:
        float raw_;
    };

    inline auto Radians::operator=(const float &f) -> Radians & {
        raw_ = f;
        return *this;
    }

    inline auto Radians::operator+=(const Radians &r) -> Radians & {
        raw_ += r.raw_;
        return *this;
    }

    inline auto Radians::operator-=(const Radians &r) -> Radians & {
        raw_ -= r.raw_;
        return *this;
    }

    inline auto Radians::operator*=(float f) -> Radians & {
        raw_ *= f;
        return *this;
    }

    inline auto Radians::operator/=(float f) -> Radians & {
        raw_ /= f;
        return *this;
    }
}