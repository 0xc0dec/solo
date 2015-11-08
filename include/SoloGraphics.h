#pragma once

#include "SoloBase.h"

namespace solo
{
	class RenderTarget;
	class Texture2D;
	class Material;
	class Device;
	class ResourceManager;
	class Mesh;

	class Graphics
	{
	public:
		static shared<Graphics> create(Device *device);

		Graphics(const Graphics& other) = delete;
		Graphics(Graphics&& other) = delete;
		Graphics& operator=(const Graphics& other) = delete;
		Graphics& operator=(Graphics&& other) = delete;
		virtual ~Graphics() {}

		virtual void renderMaterialSurfaceToTarget(Material* material, RenderTarget* target) = 0;
		virtual void renderMaterialSurfaceToScreen(Material* material) = 0;

	protected:
		explicit Graphics(Device *device);

		Device *device;
	};
}