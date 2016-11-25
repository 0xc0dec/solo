/*
    Copyright (c) Aleksey Fedotov

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
        claim that you wrote the original software. If you use this software
        in a product, an acknowledgment in the product documentation would be
        appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be
        misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
*/

#pragma once

#include <vector>


namespace solo
{
    enum class VertexBufferLayoutSemantics
    {
        Position = 0,
        Normal,
        Color,
        Tangent,
        Binormal,
        TexCoord0,
        TexCoord1,
        TexCoord2,
        TexCoord3,
        TexCoord4,
        TexCoord5,
        TexCoord6,
        TexCoord7
    };

    struct VertexBufferLayoutElement final
    {
        VertexBufferLayoutSemantics semantics;
        uint32_t size;
    };

    class VertexBufferLayout final
    {
    public:
        void add(VertexBufferLayoutSemantics semantics, uint32_t size);

        auto getElementCount() const -> uint32_t;
        auto getElement(uint32_t index) const -> const VertexBufferLayoutElement&;

        auto getSize() const -> uint32_t;

    private:
        std::vector<VertexBufferLayoutElement> elements;
        uint32_t size = 0;
    };

    inline auto VertexBufferLayout::getSize() const -> uint32_t
    {
        return size;
    }

    inline auto VertexBufferLayout::getElement(uint32_t index) const -> const VertexBufferLayoutElement &
    {
        return elements[index];
    }

    inline auto VertexBufferLayout::getElementCount() const -> uint32_t
    {
        return static_cast<uint32_t>(elements.size());
    }

    inline void VertexBufferLayout::add(VertexBufferLayoutSemantics semantics, uint32_t size)
    {
        elements.push_back(VertexBufferLayoutElement{ semantics, size });
        this->size += sizeof(float) * size;
    }
}
