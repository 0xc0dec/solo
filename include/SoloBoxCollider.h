#pragma once

#include "SoloCollider.h"


namespace solo
{
    class Vector3;

    class BoxCollider: public Collider
    {
    public:
        static auto create(const Node& node, const Vector3& size) -> sptr<BoxCollider>;

    protected:
        explicit BoxCollider(const Node& node);
    };
}