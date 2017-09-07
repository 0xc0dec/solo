/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloCommon.h"

#ifdef SL_OPENGL_RENDERER

#include "SoloOpenGL.h"
#include "SoloTexture.h"

namespace solo
{
    namespace gl
    {
        class Texture
        {
        public:
            SL_DISABLE_COPY_AND_MOVE(Texture)

            Texture();
            virtual ~Texture();

            auto getHandle() const -> GLuint { return handle; }

            virtual void bind() = 0;

        protected:
            GLuint handle = 0;
        };

        class Texture2d final: public solo::Texture2d, public Texture
        {
        public:
            Texture2d(Texture2dData *data);

            void bind() override final;
            void generateMipmaps() override final;

        private:
            void setData(const void *data);
        };

        class CubeTexture final : public solo::CubeTexture, public Texture
        {
        public:
            CubeTexture(CubeTextureData *data);

            void bind() override final;
            void generateMipmaps() override final;

        protected:
            void setData(CubeTextureData *data);
        };
    }
}

#endif