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
		void renderImage(shared<Texture2D> source, RenderTarget *target, Material *material, const std::string &textureParameterName = "mainTexture");

	private:
		friend class GraphicsFactory;

		explicit Graphics(Device *device);

		Device *device;
		shared<Mesh> quadMesh;
	};

	class GraphicsFactory
	{
		friend class Device;
		static shared<Graphics> create(Device *device);
	};
}