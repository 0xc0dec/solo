#pragma once

#include "SoloComponent.h"

namespace solo
{
	class Transform;
	class Device;

	class Spectator: public ComponentBase<Spectator>
	{
	public:
		explicit Spectator(const Node& node);

		virtual void init() override;
		virtual void update() override;

		float getMovementSpeed() const;
		void setMovementSpeed(float speed);

		float getHorizontalRotationSpeed() const;
		void setHorizontalRotationSpeed(float speed);
		
		float getVerticalRotationSpeed() const;
		void setVerticalRotationSpeed(float speed);

	private:
		Device *device = nullptr;
		Transform *transform = nullptr;

		float horizontalRotationSpeed = 0.5f;
		float verticalRotationSpeed = 0.5f;
		float movementSpeed = 10;
	};
}