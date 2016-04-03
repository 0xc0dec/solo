#pragma once

#include "SoloComponent.h"


namespace solo
{
    class Transform;
    class Device;

    class Spectator final: public ComponentBase<Spectator>
    {
    public:
        explicit Spectator(const Node& node);

        virtual void init() override final;
        virtual void update() override final;

        auto getMovementSpeed() const -> float;
        void setMovementSpeed(float speed);

        auto getHorizontalRotationSpeed() const -> float;
        void setHorizontalRotationSpeed(float speed);

        auto getVerticalRotationSpeed() const -> float;
        void setVerticalRotationSpeed(float speed);

    private:
        Device* device = nullptr;
        Transform* transform = nullptr;

        float horizontalRotationSpeed = 0.5f;
        float verticalRotationSpeed = 0.5f;
        float movementSpeed = 10;
    };

    inline auto Spectator::getHorizontalRotationSpeed() const -> float
    {
        return horizontalRotationSpeed;
    }

    inline void Spectator::setHorizontalRotationSpeed(float speed)
    {
        horizontalRotationSpeed = speed;
    }

    inline auto Spectator::getVerticalRotationSpeed() const -> float
    {
        return verticalRotationSpeed;
    }

    inline void Spectator::setVerticalRotationSpeed(float speed)
    {
        verticalRotationSpeed = speed;
    }

    inline auto Spectator::getMovementSpeed() const -> float
    {
        return movementSpeed;
    }

    inline void Spectator::setMovementSpeed(float speed)
    {
        movementSpeed = speed;
    }
}