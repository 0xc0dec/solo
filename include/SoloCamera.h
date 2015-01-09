#pragma once

#include "SoloComponent.h"
#include "SoloVector4.h"
#include "SoloDirty.h"

namespace solo
{
	class Device;

	class Camera : public ComponentBase<Camera>, public Dirty
	{
	public:
		virtual ~Camera() {}

		static ptr<Camera> create();

		virtual void update() override;
		virtual void render() override;

		void setClearColor(float r, float g, float b, float a);

		void setViewport(float left, float top, float width, float height);
		Vector4 getViewport() const;
	
	protected:
		Camera();

		ptr<Device> _device;

		Vector4 _viewport; // all values in range 0..1
		Vector4 _clearColor;

		virtual void applyViewportChange() = 0;
		virtual void applyClearColor() = 0;
		virtual void clear() = 0;
	};
}
