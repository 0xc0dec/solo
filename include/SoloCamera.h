#pragma once

#include "SoloComponent.h"
#include "SoloVector4.h"
#include "SoloDirty.h"
#include "SoloMatrix.h"

namespace solo
{
	class Transform;

	class Camera : public ComponentBase<Camera>, public Dirty
	{
	public:
		virtual ~Camera() {}

		static ptr<Camera> create(size_t node);

		virtual void update() override;
		virtual void render(const RenderContext& context) override;

		void setClearColor(float r, float g, float b, float a);

		void setViewport(float left, float top, float width, float height);
		Vector4 getViewport() const;

		void setPerspective(bool perspective);
		bool isPerspective() const;

		float getNear() const;
		float getFar() const;
		float getFOV() const;
		float getWidth() const;
		float getHeight() const;
		float getAspectRatio() const;
		void setNear(float near);
		void setFar(float far);
		void setFOV(float fov);
		void setWidth(float width);
		void setHeight(float height);
		void setAspectRatio(float ratio);

		const Matrix& getViewMatrix();
		const Matrix& getInverseViewMatrix();
		const Matrix& getProjectionMatrix();
		const Matrix& getViewProjectionMatrix();
		const Matrix& getInverseViewProjectionMatrix();

	protected:
		Camera(size_t node);

		virtual void applyViewportChange() = 0;
		virtual void applyClearColor() = 0;
		virtual void clear() = 0;

		ptr<Transform> _transform;

		bool _ortho;

		Vector4 _viewport; // all values in range 0..1
		Vector4 _clearColor;
		float _fov;
		float _near;
		float _far;
		float _width;
		float _height;
		float _aspectRatio;
		
		Matrix _viewMatrix;
		Matrix _projectionMatrix;
		Matrix _viewProjectionMatrix;
		Matrix _inverseViewMatrix;
		Matrix _inverseViewProjectionMatrix;
	};
}
