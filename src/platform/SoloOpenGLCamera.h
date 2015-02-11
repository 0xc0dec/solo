#pragma once

#include "SoloCamera.h"

namespace solo
{
	class Device;

	class OpenGLCamera: public Camera
	{
	public:
		static ptr<OpenGLCamera> create(Node* node);

	protected:
		OpenGLCamera(Node* node);

		Device* device;

		void applyViewportChange() override;
		void applyClearColor() override;
		void clear() override;
	};
}