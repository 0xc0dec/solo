#pragma once

namespace solo
{
    class BitFlags
    {
    public:
        BitFlags() {}

        auto getRaw() const -> uint32_t;

        bool isEmpty() const;
        bool checkAndRemove(uint32_t flags);
        bool isSet(uint32_t flags) const;

        void setAll();
        void set(uint32_t flags);
        void add(uint32_t flags);
        void remove(uint32_t flags);
        void clear();

    private:
        uint32_t flags = 0;
    };

    inline auto BitFlags::getRaw() const -> uint32_t
    {
        return flags;
    }

    inline bool BitFlags::isEmpty() const
    {
        return flags == 0;
    }

    inline bool BitFlags::checkAndRemove(uint32_t flags)
    {
        auto result = isSet(flags);
        remove(flags);
        return result;
    }

    inline bool BitFlags::isSet(uint32_t flags) const
    {
        return (this->flags & flags) != 0;
    }

    inline void BitFlags::setAll()
    {
        flags = ~0;
    }

    inline void BitFlags::set(uint32_t flags)
    {
        this->flags = flags;
    }

    inline void BitFlags::add(uint32_t flags)
    {
        this->flags |= flags;
    }

    inline void BitFlags::remove(uint32_t flags)
    {
        this->flags &= ~flags;
    }

    inline void BitFlags::clear()
    {
        flags = 0;
    }


    // Enable using enum classes as bit flags
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
