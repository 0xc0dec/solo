#pragma once

#include "SoloCamera.h"

namespace solo
{
	class Device;

	class OpenGLCamera: public Camera
	{
	protected:
		OpenGLCamera(Node* node);

	private:
		friend class CameraFactory;

		Device* device = nullptr;

		void applyViewportChange() override;
		void applyClearColor() override;
		void clear() override;
	};
}