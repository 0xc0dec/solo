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

#include "SoloCommon.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloRectTexture.h"
#include "SoloOpenGLTexture.h"
#include "SoloOpenGLRenderer.h"

namespace solo
{
    class Device;

    namespace gl
    {
        class RectTexture final : public solo::RectTexture, public Texture
        {
        public:
            explicit RectTexture(Device *device);

            void bind() override final;
            void generateMipmaps() override final;
            void setData(TextureFormat format, const uint8_t *data, uint32_t width, uint32_t height) override final;

            auto getHandle() const -> uint32_t;

        private:
            Renderer *renderer = nullptr;
            uint32_t handle = EmptyHandle;
        };

        inline auto RectTexture::getHandle() const -> uint32_t
        {
            return handle;
        }
    }
}

#endif
