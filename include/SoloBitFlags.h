#pragma once

namespace solo
{
    // TODO remove with std::bitset
    class BitFlags
    {
    public:
        BitFlags() {}

        unsigned getRaw() const;

        bool isEmpty() const;
        bool checkAndRemove(unsigned flags);
        bool isSet(unsigned flags) const;

        void setAll();
        void set(unsigned flags);
        void add(unsigned flags);
        void remove(unsigned flags);
        void clear();

    private:
        unsigned flags = 0;
    };

    inline unsigned BitFlags::getRaw() const
    {
        return flags;
    }

    inline bool BitFlags::isEmpty() const
    {
        return flags == 0;
    }

    inline bool BitFlags::checkAndRemove(unsigned flags)
    {
        auto result = isSet(flags);
        remove(flags);
        return result;
    }

    inline bool BitFlags::isSet(unsigned flags) const
    {
        return (this->flags & flags) != 0;
    }

    inline void BitFlags::setAll()
    {
        flags = ~0;
    }

    inline void BitFlags::set(unsigned flags)
    {
        this->flags = flags;
    }

    inline void BitFlags::add(unsigned flags)
    {
        this->flags |= flags;
    }

    inline void BitFlags::remove(unsigned flags)
    {
        this->flags &= ~flags;
    }

    inline void BitFlags::clear()
    {
        flags = 0;
    }


    // Enable using enum classes as bit flags
    template <class T, class TBase = std::underlying_type_t<T>, class = std::enable_if<std::is_enum<T>::value, T>>
    inline TBase operator |(T a, T b)
    {
        return static_cast<TBase>(a) | static_cast<TBase>(b);
    }

    template <class T, class TBase = std::underlying_type_t<T>, class = std::enable_if<std::is_enum<T>::value, T>>
    inline TBase operator &(TBase a, T b)
    {
        return a & static_cast<TBase>(b);
    }

    template <class T, class TBase = std::underlying_type_t<T>, class = std::enable_if<std::is_enum<T>::value, T>>
    inline TBase& operator |=(TBase& a, T b)
    {
        a = a | static_cast<TBase>(b);
        return a;
    }
}
