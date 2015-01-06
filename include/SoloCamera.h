#pragma once

#include "SoloComponent.h"
#include "SoloVector4.h"
#include "SoloDirty.h"

namespace solo
{
	class VideoDriver;
	class Device;

	class Camera : public ComponentBase<Camera>, public Dirty
	{
	public:
		Camera();
		virtual ~Camera() {}

		virtual void update() override;
		virtual void render() override;

		void setViewport(float left, float top, float width, float height);
		Vector4 getViewport() const;

		void setClearColor(float r, float g, float b, float a);

	private:
		ptr<VideoDriver> _driver;
		ptr<Device> _device;

		Vector4 _viewport; // all values in range 0..1
		Vector4 _clearColor;
	};
}
