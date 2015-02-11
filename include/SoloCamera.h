#pragma once

#include "SoloComponent.h"
#include "SoloVector4.h"
#include "SoloDirty.h"
#include "SoloMatrix.h"
#include "SoloTransform.h"

namespace solo
{
	class Transform;
	class Node;

	class Camera : public ComponentBase<Camera>, protected TransformCallback, Dirty
	{
	public:
		virtual ~Camera();

		static ptr<Camera> create(Node* node);

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
		Camera(Node* node);

		virtual void onTransformChanged() override;

		virtual void applyViewportChange() = 0;
		virtual void applyClearColor() = 0;
		virtual void clear() = 0;

		Transform* transform;

		bool ortho;

		Vector4 viewport; // all values in range 0..1
		Vector4 clearColor;
		float fov;
		float near;
		float far;
		float width;
		float height;
		float aspectRatio;
		
		Matrix viewMatrix;
		Matrix projectionMatrix;
		Matrix viewProjectionMatrix;
		Matrix inverseViewMatrix;
		Matrix inverseViewProjectionMatrix;
	};
}
