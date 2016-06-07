#include "SoloSpectator.h"
#include "SoloTransform.h"
#include "SoloDevice.h"
#include "SoloRadian.h"

#include <iostream>

using namespace solo;


Spectator::Spectator(const Node& node):
    ComponentBase(node),
    device(Device::get())
{
}


void Spectator::init()
{
    transform = node.getComponent<Transform>();
}


void Spectator::update()
{
    auto mouseMotion = device->getMouseMotion();
    auto dt = device->getTimeDelta();

    if (device->isMouseButtonDown(MouseButton::Left, true))
        device->setCursorCaptured(true);
    if (device->isMouseButtonReleased(MouseButton::Left))
        device->setCursorCaptured(false);

    if (device->isMouseButtonDown(MouseButton::Left, false))
    {
        if (mouseMotion.x != 0)
            transform->rotate(Vector3::unitY(), Radian(verticalRotationSpeed * dt * -mouseMotion.x), TransformSpace::World);
        if (mouseMotion.y != 0)
        {
            auto angleToUp = Vector3::angle(transform->getLocalForward(), Vector3::unitY()).getRawRadians();
            auto delta = horizontalRotationSpeed * dt * -mouseMotion.y;
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

            transform->rotate(Vector3::unitX(), Radian(delta), TransformSpace::Self);
        }
    }

    auto movement = Vector3::zero();
    if (device->isKeyPressed(KeyCode::W, false)) movement += transform->getLocalForward();
    if (device->isKeyPressed(KeyCode::S, false)) movement += transform->getLocalBack();
    if (device->isKeyPressed(KeyCode::A, false)) movement += transform->getLocalLeft();
    if (device->isKeyPressed(KeyCode::D, false)) movement += transform->getLocalRight();
    if (device->isKeyPressed(KeyCode::Q, false)) movement += transform->getLocalDown();
    if (device->isKeyPressed(KeyCode::E, false)) movement += transform->getLocalUp();
    movement.normalize();
    movement *= dt * movementSpeed;

    transform->translateLocal(movement);
}
