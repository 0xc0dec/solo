#pragma once

#include "SoloCamera.h"

namespace solo
{
	class Device;

	class OpenGLCamera: public Camera
	{
	protected:
		friend class CameraFactory;

		OpenGLCamera(Node* node);

		Device* device;

		void applyViewportChange() override;
		void applyClearColor() override;
		void clear() override;
	};
}