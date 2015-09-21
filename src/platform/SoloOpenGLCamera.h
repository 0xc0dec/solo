#pragma once

#include "SoloCamera.h"
#include "SoloNode.h"

namespace solo
{
	class Device;
	class Scene;

	class OpenGLCamera: public Camera
	{
	public:
		explicit OpenGLCamera(Scene *scene, Node node);

		virtual void init() override;

	private:
		Device* device = nullptr;

		void applyViewport() override;
		void applyClearColor() override;
		void clear() override;
	};
}