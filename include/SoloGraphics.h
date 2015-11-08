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
		Graphics(const Graphics& other) = delete;
		Graphics(Graphics&& other) = delete;
		Graphics& operator=(const Graphics& other) = delete;
		Graphics& operator=(Graphics&& other) = delete;
		virtual ~Graphics() {}

		virtual void drawMaterialToTarget(Material* material, RenderTarget* target) = 0;
		virtual void drawMaterialToScreen(Material* material) = 0;

	protected:
		explicit Graphics(Device *device);

		Device *device;
	};

	class GraphicsFactory
	{
		friend class Device;
		static shared<Graphics> create(Device *device);
	};
}