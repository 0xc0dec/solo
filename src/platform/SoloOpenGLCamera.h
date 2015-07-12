#pragma once

#include "SoloCamera.h"

namespace solo
{
	class Device;

	class OpenGLCamera: public Camera
	{
	public:
		OpenGLCamera(Scene* scene, size_t nodeId);

	private:
		Device* device = nullptr;

		void applyViewport() override;
		void applyClearColor() override;
		void clear() override;
	};
}