#pragma once

#include "SoloCollider.h"
#include "SoloVector3.h"


namespace solo
{
    class BoxCollider: public Collider
    {
    public:
        static auto create(const Node& node, const Vector3& size) -> uptr<BoxCollider>;

    protected:
        explicit BoxCollider(const Node& node);
    };
}