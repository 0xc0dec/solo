#pragma once

#include "SoloBase.h"

namespace solo
{
	class Texture2D;

	class RenderTarget
	{
	public:
		virtual ~RenderTarget() {}

		virtual void bind() = 0;
		virtual void unbind() = 0;

		void setTextures(const std::vector<shared<Texture2D>> &textures);

	protected:
		RenderTarget() {}

		virtual void applyTextures(const std::vector<shared<Texture2D>> &textures) = 0;

	private:
		friend class RenderTargetFactory;

		RenderTarget(const RenderTarget& other) = delete;
		RenderTarget(RenderTarget&& other) = delete;
		RenderTarget& operator=(const RenderTarget& other) = delete;
		RenderTarget& operator=(RenderTarget&& other) = delete;

		std::vector<shared<Texture2D>> textures;
	};

	class RenderTargetFactory
	{
		friend class ResourceManager;
		static shared<RenderTarget> create();
	};
}
