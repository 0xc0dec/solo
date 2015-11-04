#pragma once

#include "SoloBase.h"

namespace solo
{
	class RenderTarget;
	class Texture2D;
	class Material;
	class Device;
	class ResourceManager;

	class Graphics
	{
	public:
		void renderImage(Texture2D *source, RenderTarget *target, Material *material);

	private:
		friend class GraphicsFactory;

		explicit Graphics(Device *device);

		Device *device;
		ResourceManager *resourceManager;
	};

	class GraphicsFactory
	{
		friend class Device;
		static shared<Graphics> create(Device *device);
	};
}