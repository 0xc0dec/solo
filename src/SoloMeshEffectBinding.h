#pragma once

#include "SoloBase.h"

namespace solo
{
    enum class DeviceMode;
    class Mesh;
    class Effect;

    class MeshEffectBinding
    {
    public:
        static shared<MeshEffectBinding> create(DeviceMode mode, Mesh* mesh, Effect* effect);

        SL_NONCOPYABLE(MeshEffectBinding);
        virtual ~MeshEffectBinding() {}

        virtual void bind() = 0;
        virtual void unbind() = 0;

    protected:
        MeshEffectBinding() {}
    };
}
