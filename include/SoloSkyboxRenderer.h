#pragma once

#include "SoloBase.h"
#include "SoloComponent.h"
#include "SoloNode.h"
#include "SoloRenderQueue.h"

namespace solo
{
	class Mesh;
	class Material;
	class CubeTexture;

	class SkyboxRenderer: public ComponentBase<SkyboxRenderer>
	{
	public:
		static shared<SkyboxRenderer> create(Node node);

		virtual void render(RenderContext& context) override;

		virtual unsigned getRenderQueue() override;

		void setTexture(shared<CubeTexture> texture);
		shared<CubeTexture> getTexture() const;

	private:
		explicit SkyboxRenderer(Node node);

		shared<Mesh> quadMesh;
		shared<Material> material;
		shared<CubeTexture> texture;
	};

	inline unsigned SkyboxRenderer::getRenderQueue()
	{
		return KnownRenderQueues::Skyboxes;
	}

	inline shared<CubeTexture> SkyboxRenderer::getTexture() const
	{
		return texture;
	}
}