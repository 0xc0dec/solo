#include "SoloDegree.h"
#include "SoloRadian.h"
#include "SoloMath.h"

using namespace solo;


solo::Degree::Degree(const Radian& r):
    raw(r.getRawDegrees())
{
}


auto Degree::operator=(const Radian& r) -> Degree&
{
    raw = r.getRawDegrees();
    return *this;
}


auto Degree::getRawRadians() const -> float
{
    return Math::degToRad(raw);
}


auto Degree::operator+(const Radian& r) -> Degree
{
    return Degree(raw + r.getRawDegrees());
}


auto Degree::operator+=(const Radian& r) -> Degree&
{
    raw += r.getRawDegrees();
    return *this;
}


auto Degree::operator-(const Radian& r) -> Degree
{
    return Degree(raw - r.getRawDegrees());
}


auto Degree::operator-=(const Radian& r) -> Degree&
{
    raw -= r.getRawDegrees();
    return *this;
}
