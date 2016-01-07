#pragma once

#include "SoloBase.h"

namespace solo
{
    enum class DeviceMode;
    class Mesh2;
    class Effect;

    class MeshEffectBinding
    {
    public:
        static shared<MeshEffectBinding> create(DeviceMode mode, Mesh2 *mesh, Effect *effect);

        SL_NONCLONABLE(MeshEffectBinding);
        virtual ~MeshEffectBinding() {}

        virtual void bind() = 0;
        virtual void unbind() = 0;

    protected:
        MeshEffectBinding() {}
    };
}
