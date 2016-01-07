#pragma once

#include "../../SoloMeshEffectBinding.h"

namespace solo
{
    class StubMeshEffectBinding: public MeshEffectBinding
    {
    public:
        virtual ~StubMeshEffectBinding() {}

        virtual void bind() override {}
        virtual void unbind() override {}

    private:
        friend class MeshEffectBinding;

        StubMeshEffectBinding(Mesh *mesh, Effect *effect) {}
    };
}
