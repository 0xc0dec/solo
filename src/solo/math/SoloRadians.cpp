/*
 * Copyright (c) Aleksey Fedotov
 * MIT license
 */

#include "SoloRadians.h"
#include "SoloDegrees.h"
#include "SoloMath.h"

using namespace solo;

Radians::Radians(const Degrees &d):
    raw_(d.toRawRadians())
{
}

auto Radians::operator=(const Degrees &d) -> Radians &
{
    raw_ = d.toRawRadians();
    return *this;
}

auto Radians::toRawDegrees() const -> float
{
    return glm::degrees(raw_);
}

auto Radians::operator+(const Degrees &d) const -> Radians
{
    return Radians(raw_ + d.toRawRadians());
}

auto Radians::operator+=(const Degrees &d) -> Radians &
{
    raw_ += d.toRawRadians();
    return *this;
}

auto Radians::operator-(const Degrees &d) const -> Radians
{
    return Radians(raw_ - d.toRawRadians());
}

auto Radians::operator-=(const Degrees &d) -> Radians &
{
    raw_ -= d.toRawRadians();
    return *this;
}
