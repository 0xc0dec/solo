/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloSpectator.h"
#include "SoloTransform.h"
#include "SoloDevice.h"
#include "SoloRadians.h"

using namespace solo;

Spectator::Spectator(const Node &node):
    ComponentBase(node),
    device_(node.getScene()->getDevice())
{
}

void Spectator::init()
{
    transform_ = node_.findComponent<Transform>();
}

void Spectator::update()
{
    const auto mouseMotion = device_->mouseMotion();
    const auto dt = device_->timeDelta();

    if (device_->isMouseButtonDown(MouseButton::Right, true))
        device_->setCursorCaptured(true);
    if (device_->isMouseButtonReleased(MouseButton::Right))
        device_->setCursorCaptured(false);

    if (device_->isMouseButtonDown(MouseButton::Right, false))
    {
        if (mouseMotion.x() != 0)
            transform_->rotateByAxisAngle({0, 1, 0}, Radians(mouseSensitivity_ * -mouseMotion.x()), TransformSpace::World);

        if (mouseMotion.y() != 0)
        {
            const auto angleToUp = transform_->getLocalForward().angle({0, 1, 0}).toRawRadians();
            auto delta = mouseSensitivity_ * -mouseMotion.y();
            if (delta > 0)
            {
                if (angleToUp - delta <= 0.1f)
                    delta = angleToUp - 0.1f;
            }
            else
            {
                if (angleToUp - delta >= 3.04f)
                    delta = angleToUp - 3.04f;
            }

            transform_->rotateByAxisAngle({1, 0, 0}, Radians(delta), TransformSpace::Self);
        }
    }

    auto movement = Vector3();
    if (device_->isKeyPressed(KeyCode::W, false))
        movement += transform_->getLocalForward();
    if (device_->isKeyPressed(KeyCode::S, false))
        movement += transform_->getLocalBack();
    if (device_->isKeyPressed(KeyCode::A, false))
        movement += transform_->getLocalLeft();
    if (device_->isKeyPressed(KeyCode::D, false))
        movement += transform_->getLocalRight();
    if (device_->isKeyPressed(KeyCode::Q, false))
        movement += transform_->getLocalDown();
    if (device_->isKeyPressed(KeyCode::E, false))
        movement += transform_->getLocalUp();
    movement.normalize();
    movement *= dt * movementSpeed_;

    transform_->translateLocal(movement);
}
