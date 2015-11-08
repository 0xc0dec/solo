#pragma once

#include "SoloComponent.h"
#include "SoloVector4.h"
#include "SoloBitFlags.h"
#include "SoloMatrix.h"
#include "SoloTransform.h"
#include "SoloNode.h"

namespace solo
{
	class Transform;
	class Node;
	class RenderTarget;
	class Scene;
	enum class DeviceMode;

	class Camera : public ComponentBase<Camera>, protected TransformCallback
	{
	public:
		static shared<Camera> create(DeviceMode mode, Scene *scene, Node node);

		virtual void init() override;
		virtual void terminate() override;

		void apply();
		void finish();

		BitFlags& getRenderTags();

		unsigned getRenderOrder() const;
		void setRenderOrder(unsigned order);

		shared<RenderTarget> getRenderTarget() const;
		void setRenderTarget(shared<RenderTarget> target);

		Vector4 getClearColor() const;
		void setClearColor(float r, float g, float b, float a);

		Vector4 getViewport() const;
		void setViewport(float left, float top, float width, float height);
		void resetViewport();

		bool isPerspective() const;
		void setPerspective(bool perspective);

		float getNear() const;
		void setNear(float near);

		float getFar() const;
		void setFar(float far);
		
		float getFOV() const;
		void setFOV(float fov);
		
		float getWidth() const;
		void setWidth(float width);
		
		float getHeight() const;
		void setHeight(float height);
		
		float getAspectRatio() const;
		void setAspectRatio(float ratio);

		const Matrix& getViewMatrix();
		const Matrix& getInverseViewMatrix();
		const Matrix& getProjectionMatrix();
		const Matrix& getViewProjectionMatrix();
		const Matrix& getInverseViewProjectionMatrix();

	protected:
		Camera(Scene *scene, Node node);

		virtual void onTransformChanged(const Transform* transform) override;

		virtual void applyViewport() = 0;
		virtual void applyClearColor() = 0;
		virtual void clear() = 0;

		BitFlags dirtyFlags;
		BitFlags renderTags;

		unsigned renderOrder = 0;

		Scene *scene;

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

	inline unsigned Camera::getRenderOrder() const
	{
		return renderOrder;
	}

	inline void Camera::setRenderOrder(unsigned order)
	{
		renderOrder = order;
	}

	inline void Camera::setClearColor(float r, float g, float b, float a)
	{
		clearColor = Vector4(r, g, b, a);
	}

	inline bool Camera::isPerspective() const
	{
		return !ortho;
	}

	inline float Camera::getNear() const
	{
		return nearClip;
	}

	inline float Camera::getFar() const
	{
		return farClip;
	}

	inline float Camera::getFOV() const
	{
		return fov;
	}

	inline float Camera::getWidth() const
	{
		return width;
	}

	inline float Camera::getHeight() const
	{
		return height;
	}

	inline float Camera::getAspectRatio() const
	{
		return aspectRatio;
	}

	inline BitFlags& Camera::getRenderTags()
	{
		return renderTags;
	}

	inline void Camera::setRenderTarget(shared<RenderTarget> target)
	{
		renderTarget = target;
	}

	inline Vector4 Camera::getClearColor() const
	{
		return clearColor;
	}

	inline shared<RenderTarget> Camera::getRenderTarget() const
	{
		return renderTarget;
	}

	inline void Camera::terminate()
	{
		transform->removeCallback(this);
	}

	inline void Camera::resetViewport()
	{
		viewportSet = false;
	}
}
