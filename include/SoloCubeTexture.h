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

#include "SoloTexture.h"


namespace solo
{
    enum class CubeTextureFace;

    class CubeTexture: public Texture
    {
    public:
        static auto create() -> sptr<CubeTexture>;

        virtual void setData(CubeTextureFace face, TextureFormat format, const uint8_t* data, uint32_t width, uint32_t height) = 0;

        void setWrapping(TextureWrapping wrapping) override final;

        auto getDepthWrapping() const -> TextureWrapping;
        void setDepthWrapping(TextureWrapping depthWrap);

    protected:
        CubeTexture();

        void rebuildFlags() override final;

        TextureWrapping depthWrapping = TextureWrapping::Repeat;
    };

    inline auto CubeTexture::getDepthWrapping() const -> TextureWrapping
    {
        return depthWrapping;
    }

    inline void CubeTexture::setDepthWrapping(TextureWrapping wrapping)
    {
        this->depthWrapping = wrapping;
    }
}