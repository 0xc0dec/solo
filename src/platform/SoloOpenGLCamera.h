#pragma once

#include "SoloCamera.h"
#include "SoloNode.h"

namespace solo
{
	class Device;

	class OpenGLCamera: public Camera
	{
	public:
		explicit OpenGLCamera(Node node) : Camera(node) {}

		virtual void init() override;

	private:
		Device* device = nullptr;

		void applyViewport() override;
		void applyClearColor() override;
		void clear() override;
	};
}