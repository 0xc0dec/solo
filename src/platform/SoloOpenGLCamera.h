#pragma once

#include "SoloCamera.h"

namespace solo
{
	class OpenGLCamera: public Camera
	{
	public:
		static ptr<OpenGLCamera> create(size_t node);

	protected:
		OpenGLCamera(size_t node);

		void applyViewportChange() override;
		void applyClearColor() override;
		void clear() override;
	};
}