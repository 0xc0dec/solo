/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloDegrees.h"
#include "SoloRadians.h"
#include "SoloMath.h"

using namespace solo;

Degrees::Degrees(const Radians &r):
    raw(r.toRawDegrees())
{
}

auto Degrees::operator=(const Radians &r) -> Degrees &
{
    raw = r.toRawDegrees();
    return *this;
}

auto Degrees::toRawRadians() const -> float
{
    return glm::radians(raw);
}

auto Degrees::operator+(const Radians &r) const -> Degrees
{
    return Degrees(raw + r.toRawDegrees());
}

auto Degrees::operator+=(const Radians &r) -> Degrees &
{
    raw += r.toRawDegrees();
    return *this;
}

auto Degrees::operator-(const Radians &r) const -> Degrees
{
    return Degrees(raw - r.toRawDegrees());
}

auto Degrees::operator-=(const Radians &r) -> Degrees &
{
    raw -= r.toRawDegrees();
    return *this;
}
