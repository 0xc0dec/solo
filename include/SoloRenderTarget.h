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

		virtual void setTexture(shared<Texture2D> texture) = 0;

	protected:
		RenderTarget() {}

	private:
		friend class RenderTargetFactory;

		RenderTarget(const RenderTarget& other) = delete;
		RenderTarget(RenderTarget&& other) = delete;
		RenderTarget& operator=(const RenderTarget& other) = delete;
		RenderTarget& operator=(RenderTarget&& other) = delete;
	};

	class RenderTargetFactory
	{
		friend class ResourceManager;
		static shared<RenderTarget> create();
	};
}
