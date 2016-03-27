#pragma once

#include "../../include/Solo.h"


class Rotator: public solo::ComponentBase<Rotator>
{
public:
    explicit Rotator(const solo::Node& node, const std::string& space, solo::Vector3 axis):
        ComponentBase<Rotator>(node),
        device(node.getScene()->getDevice()),
        axis(axis),
        space(space)
    {
    }

    virtual void init() override final
    {
        transform = node.getComponent<solo::Transform>();
    }

    virtual void update() override final
    {
        auto angle = device->getTimeDelta();
        auto rotationSpace = solo::TransformSpace::World;
        if (space == "local")
            rotationSpace = solo::TransformSpace::Self;
        transform->rotate(axis, angle, rotationSpace);
    }

private:
    solo::Transform* transform = nullptr;
    solo::Device* device;
    solo::Vector3 axis;
    std::string space;
};