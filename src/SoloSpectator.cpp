#include "SoloSpectator.h"
#include "SoloTransform.h"
#include "SoloDevice.h"

using namespace solo;


Spectator::Spectator(const Node& node):
	ComponentBase(node)
{
	device = node.getScene()->getEngine()->getDevice();
}


void Spectator::init()
{
	transform = node.getComponent<Transform>();
}


void Spectator::update()
{
	auto mouseMotion = device->getMouseMotion();
	auto dt = device->getTimeDelta();

	if (device->isMouseButtonDown(MouseButton::Right, true))
		device->setCursorCaptured(true);
	if (device->isMouseButtonReleased(MouseButton::Right))
		device->setCursorCaptured(false);

	if (device->isMouseButtonDown(MouseButton::Right, false))
	{
		if (mouseMotion.x != 0)
			transform->rotate(Vector3::unitY(), verticalRotationSpeed * dt * -mouseMotion.x, TransformSpace::World);
		if (mouseMotion.y != 0)
			transform->rotate(Vector3::unitX(), horizontalRotationSpeed * dt * -mouseMotion.y, TransformSpace::Self);
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


float Spectator::getHorizontalRotationSpeed() const
{
	return horizontalRotationSpeed;
}


void Spectator::setHorizontalRotationSpeed(float speed)
{
	horizontalRotationSpeed = speed;
}


float Spectator::getVerticalRotationSpeed() const
{
	return verticalRotationSpeed;
}


void Spectator::setVerticalRotationSpeed(float speed)
{
	verticalRotationSpeed = speed;
}


float Spectator::getMovementSpeed() const
{
	return movementSpeed;
}


void Spectator::setMovementSpeed(float speed)
{
	movementSpeed = speed;
}
