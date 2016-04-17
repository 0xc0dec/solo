#include "SoloDegree.h"
#include "SoloRadian.h"
#include "SoloMath.h"

using namespace solo;


solo::Degree::Degree(const Radian& r):
    raw(r.valueDegrees())
{
}


auto Degree::operator=(const Radian& r) -> Degree&
{
    raw = r.valueDegrees();
    return *this;
}


auto Degree::valueRadians() const -> float
{
    return Math::degToRad(raw);
}


auto Degree::operator+(const Radian& r) -> Degree
{
    return Degree(raw + r.valueDegrees());
}


auto Degree::operator+=(const Radian& r) -> Degree&
{
    raw += r.valueDegrees();
    return *this;
}


auto Degree::operator-(const Radian& r) -> Degree
{
    return Degree(raw - r.valueDegrees());
}


auto Degree::operator-=(const Radian& r) -> Degree&
{
    raw -= r.valueDegrees();
    return *this;
}
