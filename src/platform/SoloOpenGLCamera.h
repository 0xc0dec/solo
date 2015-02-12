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
		OpenGLCamera(const OpenGLCamera& other) = delete;
		OpenGLCamera(OpenGLCamera&& other) = delete;
		OpenGLCamera& operator=(const OpenGLCamera& other) = delete;
		OpenGLCamera& operator=(OpenGLCamera&& other) = delete;

		Device* device;

		void applyViewportChange() override;
		void applyClearColor() override;
		void clear() override;
	};
}