#pragma once

#include "SoloRenderTarget.h"

namespace solo
{
	class StubRenderTarget: public RenderTarget
	{
	public:
		virtual void bind() override {}
		virtual void unbind() override {}

	protected:
		virtual void update() override {}

	private:
		friend class RenderTargetFactory;

		StubRenderTarget() {}
	};
}