#pragma once

#include "SoloCamera.h"
#include "SoloNode.h"

namespace solo
{
	class Device;

	class OpenGLCamera: public Camera
	{
	public:
		OpenGLCamera(Node node);

	private:
		Device* device = nullptr;

		void applyViewport() override;
		void applyClearColor() override;
		void clear() override;
	};
}