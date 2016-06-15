#include "SoloDegree.h"
#include "SoloRadian.h"
#include "SoloMath.h"

using namespace solo;


solo::Degree::Degree(const Radian& r):
    raw(r.toRawDegree())
{
}


auto Degree::operator=(const Radian& r) -> Degree&
{
    raw = r.toRawDegree();
    return *this;
}


auto Degree::toRawRadian() const -> float
{
    return Math::degToRad(raw);
}


auto Degree::operator+(const Radian& r) -> Degree
{
    return Degree(raw + r.toRawDegree());
}


auto Degree::operator+=(const Radian& r) -> Degree&
{
    raw += r.toRawDegree();
    return *this;
}


auto Degree::operator-(const Radian& r) -> Degree
{
    return Degree(raw - r.toRawDegree());
}


auto Degree::operator-=(const Radian& r) -> Degree&
{
    raw -= r.toRawDegree();
    return *this;
}
