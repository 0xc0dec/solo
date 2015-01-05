#pragma once

#include "SoloIComponent.h"
#include "SoloVector4.h"

namespace solo
{
	class ICamera: public Component<ICamera>
	{
	public:
		virtual void setViewport(float left, float top, float width, float height) = 0;
		virtual Vector4 getViewport() const = 0;

		virtual void setClearColor(float r, float g, float b, float a) = 0;
	};
}
