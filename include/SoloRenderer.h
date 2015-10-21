#pragma once

namespace solo
{
	struct RenderContext;

	class Renderer
	{
	public:
		virtual ~Renderer() {}

		virtual void render(RenderContext& context) = 0;
	};
}