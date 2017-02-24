/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloDegree.h"
#include "SoloRadian.h"
#include "SoloMath.h"

using namespace solo;


solo::Degree::Degree(const Radian &r):
    raw(r.toRawDegree())
{
}


auto Degree::operator=(const Radian &r) -> Degree &
{
    raw = r.toRawDegree();
    return *this;
}


auto Degree::toRawRadian() const -> float
{
    return math::degToRad(raw);
}


auto Degree::operator+(const Radian &r) const -> Degree
{
    return Degree(raw + r.toRawDegree());
}


auto Degree::operator+=(const Radian &r) -> Degree &
{
    raw += r.toRawDegree();
    return *this;
}


auto Degree::operator-(const Radian &r) const -> Degree
{
    return Degree(raw - r.toRawDegree());
}


auto Degree::operator-=(const Radian &r) -> Degree &
{
    raw -= r.toRawDegree();
    return *this;
}
