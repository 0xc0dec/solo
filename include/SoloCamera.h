#pragma once

#include "SoloComponent.h"
#include "SoloVector4.h"
#include "SoloDirty.h"
#include "SoloMatrix.h"
#include "SoloTransform.h"
#include "SoloNode.h"

namespace solo
{
	class Transform;
	class Node;
	class RenderTarget;

	class Camera : public ComponentBase<Camera>, protected TransformCallback, Dirty
	{
	public:
		virtual ~Camera() {}

		virtual void init() override;
		virtual void update() override;
		virtual void render(RenderContext& context) override;
		virtual void postRender() override;
		virtual void terminate() override;

		void setRenderTarget(shared<RenderTarget> target);
		shared<RenderTarget> getRenderTarget() const;

		void setClearColor(float r, float g, float b, float a);

		void setViewport(float left, float top, float width, float height);
		void resetViewport();
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
		friend class CameraFactory;

		explicit Camera(Node node): ComponentBase(node) {}

		virtual void onTransformChanged(const Transform* transform) override;

		virtual void applyViewport() = 0;
		virtual void applyClearColor() = 0;
		virtual void clear() = 0;

		Transform *transform = nullptr;
		shared<RenderTarget> renderTarget = nullptr;

		bool ortho = false;

		Vector4 viewport;
		bool viewportSet = false;

		Vector4 clearColor{ 0, 0, 0, 1 };
		float fov = 60;
		float nearClip = 1;
		float farClip = 100;
		float width = 1;
		float height = 1;
		float aspectRatio = 1;
		
		Matrix viewMatrix;
		Matrix projectionMatrix;
		Matrix viewProjectionMatrix;
		Matrix inverseViewMatrix;
		Matrix inverseViewProjectionMatrix;
	};

	class CameraFactory
	{
		friend class Node;
		static shared<Camera> create(Node node);
	};
}
