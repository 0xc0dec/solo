#ifndef __SOLO_CAMERA_H__
#define __SOLO_CAMERA_H__

#include "SoloICamera.h"
#include "SoloVector4.h"
#include "SoloDirty.h"

namespace solo
{
	class VideoDriver;

	class Camera : public ICamera, public Dirty
	{
	public:
		Camera();

		virtual void setViewport(float left, float top, float width, float height) override;
		virtual Vector4 getViewport() const override;

		virtual void setClearColor(float r, float g, float b, float a) override;

		void setPrimary(bool primary);
		bool isPrimary() const;

		virtual void update() override;
		void render();

	private:
		ptr<VideoDriver> _driver;

		bool _primary;
		Vector4 _viewport; // all values in range 0..1
		Vector4 _clearColor;
	};
}

#endif