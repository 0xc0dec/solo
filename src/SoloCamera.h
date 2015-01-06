#pragma once

#include "SoloICamera.h"
#include "SoloVector4.h"
#include "SoloDirty.h"

namespace solo
{
	class VideoDriver;
	class Device;

	class Camera : public ICamera, public Dirty
	{
	public:
		static ptr<Camera> create();

		virtual void setViewport(float left, float top, float width, float height) override;
		virtual Vector4 getViewport() const override;

		virtual void setClearColor(float r, float g, float b, float a) override;

		void setPrimary(bool primary);
		bool isPrimary() const;

		virtual void update() override;
		virtual void render() override;

	private:
		Camera();
		virtual ~Camera() {}

		ptr<VideoDriver> _driver;
		ptr<Device> _device;

		bool _primary;
		Vector4 _viewport; // all values in range 0..1
		Vector4 _clearColor;
	};
}
