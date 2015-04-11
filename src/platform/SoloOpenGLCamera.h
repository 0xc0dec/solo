#pragma once

#include "SoloCamera.h"

namespace solo
{
	class Device;

	class OpenGLCamera: public Camera
	{
	public:
		OpenGLCamera(Node* node);

	private:
		Device* device = nullptr;

		void applyViewportChange() override;
		void applyClearColor() override;
		void clear() override;
	};
}