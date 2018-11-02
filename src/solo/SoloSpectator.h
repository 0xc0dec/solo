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

        auto movementSpeed() const -> float { return movementSpeed_; }
        void setMovementSpeed(float speed) { movementSpeed_ = speed; }

        auto mouseSensitivity() const -> float { return mouseSensitivity_; }
        void setMouseSensitivity(float sensitivity) { mouseSensitivity_ = sensitivity; }

        auto rotationAcceleration() const -> float { return rotationAcceleration_; }
        void setRotationAcceleration(float acceleration) { rotationAcceleration_ = acceleration;  }

    private:
        Device *device_ = nullptr;
        Transform *transform_ = nullptr;

        float mouseSensitivity_ = 0.002f;
        float rotationAcceleration_ = 20;
        float movementSpeed_ = 10;
    };
}