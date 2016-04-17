#include "SoloRadian.h"
#include "SoloDegree.h"
#include "SoloMath.h"

using namespace solo;


Radian::Radian(const Degree& d):
    raw(d.valueRadians())
{
}


auto Radian::operator=(const Degree& d) -> Radian&
{
    raw = d.valueRadians();
    return *this;
}


auto Radian::valueDegrees() const -> float
{
    return Math::radToDeg(raw);
}


auto Radian::operator+(const Degree& d) const -> Radian
{
    return Radian(raw + d.valueRadians());
}


auto Radian::operator+=(const Degree& d) -> Radian&
{
    raw += d.valueRadians();
    return *this;
}


auto Radian::operator-(const Degree& d) const -> Radian
{
    return Radian(raw - d.valueRadians());
}


auto Radian::operator-=(const Degree& d) -> Radian&
{
    raw -= d.valueRadians();
    return *this;
}
