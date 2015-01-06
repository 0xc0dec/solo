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
		virtual ~Camera() {}

		static ptr<Camera> create();

		virtual void update() override;
		virtual void render() override;

		void setViewport(float left, float top, float width, float height);
		Vector4 getViewport() const;

		void setClearColor(float r, float g, float b, float a);

		// TODO probably not an interface methods
		void setPrimary(bool primary);
		bool isPrimary() const;

	private:
		Camera();

		ptr<VideoDriver> _driver;
		ptr<Device> _device;

		bool _primary;
		Vector4 _viewport; // all values in range 0..1
		Vector4 _clearColor;
	};
}
