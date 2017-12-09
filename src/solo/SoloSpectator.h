/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#pragma once

#include "SoloComponent.h"

namespace solo
{
    class Transform;
    class Device;

    class Spectator final: public ComponentBase<Spectator>
    {
    public:
        explicit Spectator(const Node &node);

        void init() override final;
        void update() override final;

        auto getMovementSpeed() const -> float { return movementSpeed; }
        void setMovementSpeed(float speed) { movementSpeed = speed; }

        auto getMouseSensitivity() const -> float { return mouseSensitivity; }
        void setMouseSensitivity(float sensitivity) { mouseSensitivity = sensitivity; }

        auto getRotationAcceleration() const -> float { return rotationAcceleration; }
        void setRotationAcceleration(float acceleration) { rotationAcceleration = acceleration;  }

    private:
        Device *device = nullptr;
        Transform *transform = nullptr;

        float mouseSensitivity = 0.002f;
        float rotationAcceleration = 20;
        float movementSpeed = 10;
    };
}