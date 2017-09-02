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

        auto getMovementSpeed() const -> float;
        void setMovementSpeed(float speed);

        auto getMouseSensitivity() const -> float;
        void setMouseSensitivity(float sensitivity);

        auto getRotationAcceleration() const -> float;
        void setRotationAcceleration(float acceleration);

    private:
        Device *device = nullptr;
        Transform *transform = nullptr;

        float mouseSensitivity = 0.08f;
        float rotationAcceleration = 20;
        float movementSpeed = 10;
    };

    inline auto Spectator::getMouseSensitivity() const -> float
    {
        return mouseSensitivity;
    }

    inline void Spectator::setMouseSensitivity(float sensitivity)
    {
        mouseSensitivity = sensitivity;
    }

    inline auto Spectator::getMovementSpeed() const -> float
    {
        return movementSpeed;
    }

    inline void Spectator::setMovementSpeed(float speed)
    {
        movementSpeed = speed;
    }

    inline auto Spectator::getRotationAcceleration() const -> float
    {
        return rotationAcceleration;
    }

    inline void Spectator::setRotationAcceleration(float acceleration)
    {
        rotationAcceleration = acceleration; 
    }
}