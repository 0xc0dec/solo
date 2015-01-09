#pragma once

#include "SoloCamera.h"

namespace solo
{
	class OpenGLCamera: public Camera
	{
	public:
		static ptr<OpenGLCamera> create();

	protected:
		OpenGLCamera() {}

		void applyViewportChange() override;
		void applyClearColor() override;
		void clear() override;
	};
}