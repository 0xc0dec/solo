#include "SoloRadian.h"
#include "SoloDegree.h"
#include "SoloMath.h"

using namespace solo;


Radian::Radian(const Degree& d):
    raw(d.getRawRadians())
{
}


auto Radian::operator=(const Degree& d) -> Radian&
{
    raw = d.getRawRadians();
    return *this;
}


auto Radian::getRawDegrees() const -> float
{
    return Math::radToDeg(raw);
}


auto Radian::operator+(const Degree& d) const -> Radian
{
    return Radian(raw + d.getRawRadians());
}


auto Radian::operator+=(const Degree& d) -> Radian&
{
    raw += d.getRawRadians();
    return *this;
}


auto Radian::operator-(const Degree& d) const -> Radian
{
    return Radian(raw - d.getRawRadians());
}


auto Radian::operator-=(const Degree& d) -> Radian&
{
    raw -= d.getRawRadians();
    return *this;
}
