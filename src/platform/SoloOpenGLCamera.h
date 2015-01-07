#pragma once

#include "SoloCamera.h"

namespace solo
{
	class OpenGLCamera: public Camera
	{
	protected:
		void applyViewportChange() override;
		void applyClearColor() override;
		void clear() override;
	};
}