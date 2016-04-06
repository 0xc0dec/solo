#pragma once

namespace solo
{
    // Enable usage of enum classes as bit flags

    template <class T, class TBase = std::underlying_type_t<T>, class = std::enable_if<std::is_enum<T>::value, T>>
    inline auto operator |(T a, T b) -> TBase
    {
        return static_cast<TBase>(a) | static_cast<TBase>(b);
    }

    template <class T, class TBase = std::underlying_type_t<T>, class = std::enable_if<std::is_enum<T>::value, T>>
    inline auto operator &(TBase a, T b) -> TBase
    {
        return a & static_cast<TBase>(b);
    }

    template <class T, class TBase = std::underlying_type_t<T>, class = std::enable_if<std::is_enum<T>::value, T>>
    inline auto operator |=(TBase& a, T b) -> TBase&
    {
        a = a | static_cast<TBase>(b);
        return a;
    }
}
