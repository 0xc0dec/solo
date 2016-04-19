#pragma once

#include "SoloComponent.h"


namespace solo
{
    class Collider: public ComponentBase<Collider>
    {
    public:

    protected:
        Collider(const Node& node);
    };
}