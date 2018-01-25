/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloRadians.h"
#include "SoloDegrees.h"
#include "SoloMath.h"

using namespace solo;

Radians::Radians(const Degrees &d):
    raw(d.toRawRadians())
{
}

auto Radians::operator=(const Degrees &d) -> Radians &
{
    raw = d.toRawRadians();
    return *this;
}

auto Radians::toRawDegrees() const -> float
{
    return math::radToDeg(raw);
}

auto Radians::operator+(const Degrees &d) const -> Radians
{
    return Radians(raw + d.toRawRadians());
}

auto Radians::operator+=(const Degrees &d) -> Radians &
{
    raw += d.toRawRadians();
    return *this;
}

auto Radians::operator-(const Degrees &d) const -> Radians
{
    return Radians(raw - d.toRawRadians());
}

auto Radians::operator-=(const Degrees &d) -> Radians &
{
    raw -= d.toRawRadians();
    return *this;
}
